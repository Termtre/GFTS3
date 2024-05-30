#include "../include/SOR_COL.h"

#include <algorithm>
#include <cstring>

namespace numeric_method
{

void SOR_COL::initCL()
{
    int ret = 0;
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_program program = NULL;

    ret = clGetPlatformIDs(1, &platform_id, NULL);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1, &device_id, NULL);

    context = clCreateContext(NULL, 1, &device_id, NULL, NULL,&ret);

    queue = clCreateCommandQueue(context, device_id, 0, &ret);

    const char * kernelCode =
        "__kernel void five_point_stenc(__global const double* up_left, __global double* centers, __global const double* right_down, __global const double* f, const int diagonal, "
        "const double w, const double w_a, const double x_step_2, const double y_step_2) {"
        "    int idx = get_global_id(0);"
        "    if (idx > 0 &&  idx < diagonal - 1) "
        "    {"
        "       centers[idx] = (1.0 - w) * centers[idx] + w_a * "
        "       ("
        "           f[idx] +"
        "           x_step_2 * (up_left[idx] + right_down[idx]) +"
        "           y_step_2 * (up_left[idx - 1] + right_down[idx + 1])"
        "       );"
        "    }"
        "}";
// (1.0 - w) * v[1][1] + w_a * ( f[1][1] + x_step_2 * (v[1][0] + v[1][2]) + y_step_2 * (v[2][1] + v[0][1]))

    size_t CodeLenght = strlen(kernelCode);
    program = clCreateProgramWithSource(context, 1, &kernelCode, &CodeLenght ,&ret);
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    int overclocking_count = std::min(n,m) + 1;

    up_left = clCreateBuffer(   context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count, NULL, &ret);
    centers = clCreateBuffer(   context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count, NULL, &ret);
    right_down = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count, NULL, &ret);
    fun = clCreateBuffer(       context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count, NULL, &ret);

    kernel = clCreateKernel(program, "five_point_stenc", &ret);

    // clRelease();
    // clRelease();
    // clRelease();
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
    Matrix diag(rec.size() + rec[0].size() - 1, std::vector<double>(rec.size()));

    for(size_t i = 0; i < rec.size(); ++i)
        for(size_t j = 0; j < rec[i].size(); ++j)
            diag[i + j][i] = rec[i][j];

    return diag;
};

Matrix SOR_COL::to_rectangle(const Matrix& diag)
{
    Matrix rec(n + 1, std::vector<double>(m + 1));

    for(size_t i = 0; i < rec.size(); ++i)
        for(size_t j = 0; j < rec[i].size(); ++j)
            rec[i][j] = diag[i + j][i];

    return rec;
};

int SOR_COL::solve(const double prec, const int N_max)
{
    if (n < 2 || m < 2)
        return 0;
    int N = 0;
    Matrix v_diag = to_diagonal(v);
    const Matrix f_diag = to_diagonal(f);

    int ret = 0;

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &up_left);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &centers);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &right_down);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), &fun);

    ret = clSetKernelArg(kernel, 5, sizeof(double), &w         );
    ret = clSetKernelArg(kernel, 6, sizeof(double), &w_a       );
    ret = clSetKernelArg(kernel, 7, sizeof(double), &x_step_2  );
    ret = clSetKernelArg(kernel, 8, sizeof(double), &y_step_2  );


    for(; N < N_max; ++N)
    {

        //ret = clEnqueueWriteBuffer(queue, centers,    CL_FALSE, 0, sizeof(double) * (n + 1), v_diag[1].data(),     0, NULL, NULL);
        //ret = clEnqueueWriteBuffer(queue, right_down, CL_FALSE, 0, sizeof(double) * (n + 1), v_diag[2].data(), 0, NULL, NULL);

        double eps = 0;
        for(int i = 2; i < v_diag.size() - 2; ++i)
        {
            const size_t diagonal = std::min(i, static_cast<int>(n)) + 1;
            const size_t globalOffset = std::max(0, i - static_cast<int>(m)) + 1;

            ret = clEnqueueWriteBuffer(queue, up_left,    CL_FALSE, 0, sizeof(double) * (n + 1), v_diag[i - 1].data(), 0, NULL, NULL);
            ret = clEnqueueWriteBuffer(queue, centers,    CL_FALSE, 0, sizeof(double) * (n + 1), v_diag[i].data(),     0, NULL, NULL);
            ret = clEnqueueWriteBuffer(queue, right_down, CL_FALSE, 0, sizeof(double) * (n + 1), v_diag[i + 1].data(), 0, NULL, NULL);
            ret = clEnqueueWriteBuffer(queue, fun       , CL_FALSE, 0, sizeof(double) * (n + 1), f_diag[i].data(),     0, NULL, NULL);

            /*
            ret = clEnqueueCopyBuffer(   queue, centers   , up_left, 0, 0, sizeof(double) *  (n + 1), 0, NULL, NULL );
            ret = clFinish(queue);

            ret = clEnqueueCopyBuffer(   queue, right_down, centers, 0, 0, sizeof(double) * (n + 1), 0, NULL, NULL);
            ret = clFinish(queue);*/



            size_t localWorkSize = diagonal;

            std::vector<double> eps_vec = v_diag[i];

            ret = clFinish(queue);

            ret = clSetKernelArg(kernel, 4, sizeof(int),    &diagonal);
            ret = clEnqueueNDRangeKernel(queue, kernel, 1, &globalOffset, &diagonal, &diagonal, 0, NULL, NULL);
            ret = clFinish(queue);

            ret = clEnqueueReadBuffer(   queue, centers   , CL_TRUE,   0, sizeof(double) * (n + 1) , v_diag[i].data()    , 0, NULL, NULL);

            std::transform(eps_vec.begin(), eps_vec.end(), v_diag[i].begin(), eps_vec.begin(),[](double a, double b){return std::abs(a - b);});

            eps = std::max(eps,*std::max_element(eps_vec.begin(), eps_vec.end()));
        }
        precision = eps;
        if(precision < prec)
            break;
    }

    v = to_rectangle(v_diag);

    return N;
};

}
