#include "../include/SOR_COL.h"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <fstream>
#include <omp.h>

namespace numeric_method
{

void SOR_COL::initCL()
{
    int ret = 0;
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;

    ret = clGetPlatformIDs(1, &platform_id, NULL);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1, &device_id, NULL);

    context = clCreateContext(NULL, 1, &device_id, NULL, NULL,&ret);

    queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

    const char * kernelCode = R"CLC(
       __kernel void five_point_stenc(__global const double* up_left, __global double* centers, __global const double* right_down, __global const double* f, const int diagonal,
       const double w, const double w_a, const double x_step_2, const double y_step_2)
       {
            int idx = get_global_id(0);
            if (idx > 0 &&  idx < diagonal - 1)
            {
                centers[idx] = (1.0 - w) * centers[idx] + w_a *
                (
                    f[idx] +
                    x_step_2 * (up_left[idx] + right_down[idx]) +
                    y_step_2 * (up_left[idx - 1] + right_down[idx + 1])
                );
            }
        }

        __kernel void max_eps(__global const double* start, __global const double* end, __global double* maxEps, const unsigned int numElements)
        {
        __local double localMax[256];

        unsigned int local_id = get_local_id(0);
        unsigned int global_id = get_global_id(0);
        unsigned int group_size = get_local_size(0);

        localMax[local_id] = (global_id < numElements) ? fabs(start[global_id] - end[global_id]) : -DBL_MAX;

        barrier(CLK_LOCAL_MEM_FENCE);
        for (unsigned int stride = 1; stride < group_size; stride *= 2)
        {
            barrier(CLK_LOCAL_MEM_FENCE);
            if (local_id % (2 * stride) == 0 && (local_id + stride) < group_size)
            {
                localMax[local_id] = fmax(localMax[local_id], localMax[local_id + stride]);
            }
        }
        if (local_id == 0)
            maxEps[get_group_id(0)] = localMax[0];
        };
        )CLC";

    const size_t CodeLenght = strlen(kernelCode);
    program = clCreateProgramWithSource(context, 1, &kernelCode, &CodeLenght ,&ret);

    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    const int overclocking_count = std::min(n,m) + 1;

    control = clCreateBuffer(   context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count, NULL, &ret);
    epsilon = clCreateBuffer(   context, CL_MEM_READ_WRITE, sizeof(double) * ((overclocking_count - 1) / 8 + 1) * 8, NULL, &ret);
    up_left = clCreateBuffer(   context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count, NULL, &ret);
    centers = clCreateBuffer(   context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count, NULL, &ret);
    right_down = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count, NULL, &ret);
    fun = clCreateBuffer(       context, CL_MEM_READ_WRITE, sizeof(double) * overclocking_count, NULL, &ret);

    kernel = clCreateKernel(program, "five_point_stenc", &ret);
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &up_left);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &centers);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &right_down);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &fun);

    clSetKernelArg(kernel, 5, sizeof(double), &w         );
    clSetKernelArg(kernel, 6, sizeof(double), &w_a       );
    clSetKernelArg(kernel, 7, sizeof(double), &x_step_2  );
    clSetKernelArg(kernel, 8, sizeof(double), &y_step_2  );

    diff_kernel = clCreateKernel(program, "max_eps", &ret);
    clSetKernelArg(diff_kernel, 0, sizeof(cl_mem), &control);
    clSetKernelArg(diff_kernel, 1, sizeof(cl_mem), &centers);
    clSetKernelArg(diff_kernel, 2, sizeof(cl_mem), &epsilon);

    // size_t max_work_group_size;
    // clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_work_group_size, NULL);
    //
    // size_t preferred_multiple;
    // clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &preferred_multiple, NULL);
    //

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

SOR_COL::~SOR_COL()
{
    clReleaseMemObject(up_left);
    clReleaseMemObject(centers);
    clReleaseMemObject(right_down);
    clReleaseMemObject(fun);

    clReleaseKernel(kernel);

    clReleaseProgram(program);

    clReleaseCommandQueue(queue);

    clReleaseContext(context);
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

SOR_COL::colored_thread::colored_thread(const MWR& data)
:
w(data.w),
w_a(data.w_a),
x_step_2(data.x_step_2),
y_step_2(data.y_step_2)
{
    executer = std::thread([&]()
    {
         while(solve_running)
            {
            if(calc_running == false)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            else
            {
                for(int idx = start; idx < end; ++idx)
                {
                    const double v_old = (*centers)[idx];
                    (*centers)[idx] = (1.0 - w) * (*centers)[idx] + w_a *
                    (
                    (*fun)[idx] +
                    x_step_2 * ((*up_left)[idx] + (*right_down)[idx]) +
                    y_step_2 * ((*up_left)[idx - 1] + (*right_down)[idx + 1])
                    );
                    precision = std::max(precision, std::abs(v_old - (*centers)[idx]));
                }
                calc_running = false;
            }
            }
        }
    );
};

int SOR_COL::solve(const double prec, const int N_max)
{
    if (n < 2 || m < 2)
        return 0;
    int N = 0;
    Matrix v_diag = to_diagonal(v);
    Matrix f_diag = to_diagonal(f);
//    colored_thread s(*this);
//    const size_t preferred_multiple = 8;
//    int ret = 0;
    // cl_event writeBuf;
    // cl_event doKernel;
    // cl_event readBuf;
    // cl_event epsKernel;
    // cl_ulong time_start, time_end;
//    double w_time = 0.0;
//    double k_time = 0.0;
//    double r_time = 0.0;
//    double e_time = 0.0;

    for(; N < N_max; ++N)
    {
        //ret = clEnqueueWriteBuffer(queue, centers,    CL_FALSE, 0, sizeof(double) * 2, v_diag[1].data(),     0, NULL, NULL);
        //ret = clEnqueueWriteBuffer(queue, right_down, CL_FALSE, 0, sizeof(double) * 3, v_diag[2].data(), 0, NULL, NULL);
        //s.precision = 0.0;
//        const Matrix v_cont = v_diag;
        double eps = 0.0;
        for(int i = 2; i < v_diag.size() - 2; ++i)
        {
            // ret = clEnqueueCopyBuffer(   queue, centers   , up_left, 0, 0, sizeof(double) *  (1 + diagonal - globalOffset), 0, NULL, NULL );
            // ret = clFinish(queue);
            //
            // ret = clEnqueueCopyBuffer(   queue, right_down, centers, 0, 0, sizeof(double) * (1 + diagonal - globalOffset), 0, NULL, NULL);
            // ret = clFinish(queue);



            const size_t diagonal = std::min(i, static_cast<int>(n)) + 1;


        //     const size_t global_work_group = ((diagonal - 1) / preferred_multiple + 1) * preferred_multiple;
            const size_t globalOffset = std::max(0, i - static_cast<int>(m));

            // s.up_left = &v_diag[i - 1];
            // s.centers = &v_diag[i];
            // s.right_down = &v_diag[i + 1];
            // s.fun = &f_diag[i];
            //
            // s.start = globalOffset + 1;
            // s.end = diagonal / 2;
            //
            // s.calc_running = true;

            for(int idx = globalOffset; idx < diagonal - 2; ++idx)
            {
                const double v_old = v_diag[i][idx];
                    v_diag[i][idx] = (1.0 - w) * v_diag[i][idx] + w_a *
                    (
                    f_diag[i][idx] +
                    x_step_2 * (v_diag[i-1][idx] + v_diag[i + 1][idx]) +
                    y_step_2 * (v_diag[i - 1][idx - 1] + v_diag[i + 1][idx + 1])
                    );
                    eps = std::max(eps, std::abs(v_old - v_diag[i][idx]));
            }

            // while(s.calc_running)
            //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        //
        //     ret = clEnqueueWriteBuffer(queue, up_left,    CL_FALSE, 0, sizeof(double) * (diagonal - globalOffset), v_diag[i - 1].data() + globalOffset, 0, NULL, NULL);
        //     ret = clEnqueueWriteBuffer(queue, centers,    CL_FALSE, 0, sizeof(double) * (diagonal - globalOffset), v_diag[i].data() + globalOffset,     0, NULL, NULL);
        //     //ret = clEnqueueWriteBuffer(queue, control,    CL_FALSE, 0, sizeof(double) * (diagonal - globalOffset), v_diag[i].data() + globalOffset,     0, NULL, NULL);
        //     ret = clEnqueueWriteBuffer(queue, right_down, CL_FALSE, 0, sizeof(double) * (diagonal - globalOffset), v_diag[i + 1].data() + globalOffset, 0, NULL, NULL);
        //     ret = clEnqueueWriteBuffer(queue, fun       , CL_FALSE, 0, sizeof(double) * (diagonal - globalOffset), f_diag[i].data() + globalOffset,     0, NULL, &writeBuf);
        //
        //     //std::vector<double> eps_vec (preferred_multiple);
        //     //std::vector<double> eps_vec(v_diag[i].data() + globalOffset, v_diag[i].data() + diagonal);
        //
        //     //ret = clEnqueueWriteBuffer(queue, epsilon,    CL_FALSE, 0, sizeof(double) * preferred_multiple, eps_vec.data(),     0, NULL, NULL);
        //
        //     ret = clWaitForEvents(1, &writeBuf);
        //
        //     clGetEventProfilingInfo(writeBuf, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
        //     clGetEventProfilingInfo(writeBuf, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
        //
        //     w_time += static_cast<double>(time_end - time_start);
        //
        //     clFinish(queue);
        //
        //     ret = clSetKernelArg(kernel, 4, sizeof(int),&diagonal);
        //     //ret = clSetKernelArg(diff_kernel, 3, sizeof(int),&diagonal);
        //
        //     ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_group, &preferred_multiple, 0, NULL, &doKernel);
        //
        //     ret = clWaitForEvents(1, &doKernel);
        //
        //     clGetEventProfilingInfo(doKernel, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
        //     clGetEventProfilingInfo(doKernel, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
        //
        //     k_time += static_cast<double>(time_end - time_start);
        //
        //
        //     // ret = clEnqueueNDRangeKernel(queue, diff_kernel, 1, NULL, &global_work_group, &preferred_multiple, 0,NULL, &epsKernel);
        //     // ret = clWaitForEvents(1, &epsKernel);
        //     //
        //     // clGetEventProfilingInfo(epsKernel, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
        //     // clGetEventProfilingInfo(epsKernel, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
        //     //
        //     // e_time += static_cast<double>(time_end - time_start);
        //     //
        //     // ret = clEnqueueReadBuffer(   queue, epsilon   , CL_FALSE,   0, sizeof(double) * preferred_multiple , eps_vec.data()   , 0, NULL, NULL);
        //     ret = clEnqueueReadBuffer(   queue, centers   , CL_FALSE,   0, sizeof(double) * (diagonal - globalOffset - 1) , v_diag[i].data() + globalOffset   , 0, NULL, &readBuf);
        //
        //     ret = clWaitForEvents(1, &readBuf);
        //
        //     clGetEventProfilingInfo(readBuf, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
        //     clGetEventProfilingInfo(readBuf, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
        //
        //     r_time += static_cast<double>(time_end - time_start);
        //
        //
        //     // for(size_t j = 0; j < diagonal - globalOffset; ++j)
        //     // {
        //     //   eps = std::max(eps, std::abs(eps_vec[j] - v_diag[i][j + globalOffset]));
        //     // }
        //
        //     }
        //
        // #pragma omp parallel for reduction(max:eps)
        // for (int i = 0; i < v_diag.size(); ++i)
        // {
        //     for(int j = 0; j < v_diag[i].size(); ++j)
        //     {
        //         const double diff = std::abs(v_diag[i][j] - v_cont[i][j]);
        //         if (diff > eps)
        //             eps = diff;
        //
        //     }
        // }
        precision = eps;
        if(precision < prec)
            break;
    }

    // clReleaseEvent(writeBuf);
    // clReleaseEvent(doKernel);
    // clReleaseEvent(readBuf);
    //
    // std::ofstream out("perfomance_test.txt", std::ios::app );
    // out << n << "x" << m << " w = " << w_time << " k = " << k_time << " e = "<< e_time << " r =" << r_time << std::endl;
    // out.close();
    v = to_rectangle(v_diag);

    return N;
};

}
