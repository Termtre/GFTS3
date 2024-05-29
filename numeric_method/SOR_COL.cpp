#include "../include/SOR_COL.h"

#include <algorithm>

namespace numeric_method
{

void SOR_COL::initCL()
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    auto platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    auto device = devices.front();

    cl::Context context(device);
    queue = cl::CommandQueue(context, device);

    std::string kernelCode =
        "__kernel void five_point_stenc(__global const double* up_left, __global double* centers __global const double* right_down, _global const double* f, const int diagonal, "
        "const double w, const double w_a, const double x_step_2, const double y_step_2) {"
        "    int idx = get_global_id(0);"
        "    if (idx > 1 &&  idx < diagonal - 1) "
             "{"
        "       centers[idx] = (1.0 - w) * centers[idx] + w_a * "
        "       ("
        "           f[idx] +"
        "           x_step_2 * (up_left[idx] + right_down[idx]) +"
        "           y_step_2 * (up_left[idx - 1] + right_down[idx + 1])"
        "       );"
        "    }"
        "}";

    cl::Program::Sources sources;
    sources.push_back({kernelCode.c_str(), kernelCode.length()});
    cl::Program program(context, sources);

    int overclocking_count = std::min(n,m) + 1;

    up_left = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count);
    centers = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count);
    right_down = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count);

    kernel = cl::Kernel(program, "five_point_stenc");
};

SOR_COL::SOR_COL(const size_t _n, const size_t _m)
:numeric_method::MWR(_n,_m)
{
    initCL();
};

SOR_COL::SOR_COL(const size_t _n, const size_t _m, numeric_method::test dummy)
:numeric_method::MWR(_n,_m,dummy)
{
    initCL();
};

Matrix SOR_COL::to_diagonal(const Matrix& rec)
{
    Matrix diag(rec.size() + rec[0].size() - 1);

    for(size_t i = 0; i < rec.size(); ++i)
        for(size_t j = 0; j < rec[i].size(); ++j)
            diag[i + j].push_back(v[i][j]);

    return diag;
};

Matrix SOR_COL::to_rectangle(const Matrix& diag)
{
    Matrix rec(n + 1, std::vector<double>(m + 1));

    for(size_t i = 0; i < diag.size(); ++i)
        for(size_t j = 0; j < diag[i].size(); ++j)
            rec[i - j].push_back(diag[i][j]);

    return rec;
};

int SOR_COL::solve(const double prec, const int N_max)
{
    int N = 0;
    Matrix v_diag = to_diagonal(v);
    const Matrix f_diag = to_diagonal(f);

    kernel.setArg(0, up_left);
    kernel.setArg(1, centers);
    kernel.setArg(2, right_down);
    kernel.setArg(3, fun);
    std::vector<double> param = {w, w_a, x_step_2, y_step_2};
    kernel.setArg(5, param.data());

    for(; N < N_max; ++N)
    {
        //prepare buffers
        queue.enqueueWriteBuffer(      up_left,    CL_FALSE, 0, sizeof(double) * v_diag[1].size(), v_diag[1].data());
        queue.enqueueWriteBuffer(      centers,    CL_FALSE, 0, sizeof(double) * v_diag[2].size(), v_diag[2].data());
        queue.enqueueWriteBuffer(      right_down, CL_FALSE, 0, sizeof(double) * v_diag[3].size(), v_diag[3].data());
        queue.enqueueWriteBuffer(      fun,        CL_FALSE, 0, sizeof(double) * f_diag[2].size(), f_diag[2].data());

        queue.finish();
        for(size_t i = 3; i < v_diag.size() - 2; ++i)
        {

            const int diagonal = v_diag[i].size();
            const int next_diag = v_diag[i + 1].size();
            std::vector<double> eps = v_diag[i];

            std::vector <cl::Event> event(1);

            kernel.setArg(4, diagonal);

            queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(diagonal), cl::NullRange);
            queue.finish();

            queue.enqueueCopyBuffer(   centers   , up_left, 0, 0, sizeof(double) * diagonal );
            queue.finish();

            queue.enqueueCopyBuffer(   right_down, centers, 0, 0, sizeof(double) * next_diag);
            queue.finish();

            queue.enqueueReadBuffer(   centers   , CL_FALSE,   0, sizeof(double) * diagonal , v_diag[i].data()    );
            queue.enqueueWriteBuffer(  right_down, CL_FALSE,   0, sizeof(double) * next_diag, v_diag[i + 1].data());
            queue.enqueueWriteBuffer(  fun       , CL_FALSE,   0, sizeof(double) * next_diag, f_diag[i + 1].data());
            queue.finish();

            std::transform(eps.begin(), eps.end(), v_diag[i].begin(), eps.begin(),[](double a, double b){return std::abs(a - b);});

            precision = std::max(precision,*std::max_element(eps.begin(), eps.end()));
        }

        if(precision < prec)
            break;
    }

    v = to_rectangle(v_diag);
    return N;
};

}
