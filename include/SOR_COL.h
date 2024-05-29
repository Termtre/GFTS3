#pragma once

#include <thread>

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

#include "MWR.h"

namespace numeric_method
{

class SOR_COL : public numeric_method::MWR
{
public:
    SOR_COL(const size_t _n, const size_t _m);
    SOR_COL(const size_t _n, const size_t _m, numeric_method::test dummy);
    virtual ~SOR_COL() {};
    using MWR::operator();

    int solve(const double precision, const int N_max);
private:
    void prepare_buffers();
    Matrix to_diagonal(const Matrix& rec);
    Matrix to_rectangle(const Matrix& diag);
    void initCL();
private:
    cl::CommandQueue queue;
    cl::Kernel kernel;
    cl::Buffer up_left;
    cl::Buffer centers;
    cl::Buffer right_down;
    cl::Buffer fun;
// private:
//     struct colored_thread
//     {
//         double precision = 0.0;
//         size_t i_start = 1, j_start = 1;
//         size_t count = 0;
//         std::thread executer;
//         bool solve_running = true;
//         bool calc_running = false;
//         Matrix_solver * memory;
//         colored_thread(Matrix_solver * _memory);
//         virtual ~colored_thread();
//      };

};
}
