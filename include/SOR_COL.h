#pragma once

#include <thread>

#define CL_VERSION_1_2
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl_gl.h>

#include "MWR.h"

namespace numeric_method
{

class SOR_COL : public numeric_method::MWR
{
public:
    SOR_COL(const size_t _n, const size_t _m);
    SOR_COL(const size_t _n, const size_t _m, numeric_method::test dummy);
    virtual ~SOR_COL();
    using MWR::operator();

    int solve(const double precision, const int N_max);
private:
    void prepare_buffers();
    Matrix to_diagonal(const Matrix& rec);
    Matrix to_rectangle(const Matrix& diag);
    void initCL();
private:
    cl_context context     = NULL;
    cl_program program     = NULL;
    cl_command_queue queue = NULL;
    cl_kernel kernel       = NULL;
    cl_kernel diff_kernel  = NULL;

    cl_mem control         = NULL;
    cl_mem epsilon         = NULL;
    cl_mem up_left         = NULL;
    cl_mem centers         = NULL;
    cl_mem right_down      = NULL;
    cl_mem fun             = NULL;
private:
    struct colored_thread
    {
        double precision = 0.0;
        size_t start = 0;
        size_t end = 0;
        std::thread executer;
        bool solve_running = true;
        bool calc_running = false;
        double w;
        double w_a;
        double x_step_2;
        double y_step_2;
        std::vector<double> * up_left;
        std::vector<double> * centers;
        std::vector<double> * right_down;
        std::vector<double> * fun;
        colored_thread(const MWR& data);
        virtual ~colored_thread() {};
     };

};
}
