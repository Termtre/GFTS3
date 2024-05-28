#include "../include/SOR_COL.h"

namespace numeric_method
{

SOR_COL::SOR_COL(const size_t _n, const size_t _m)
:numeric_method::MWR(_n,_m)
{};

SOR_COL::SOR_COL(const size_t _n, const size_t _m, numeric_method::test dummy)
:numeric_method::MWR(_n,_m,dummy)
{};

SOR_COL::colored_thread::colored_thread(Matrix_solver * _memory)
: memory(_memory)
{
    executer = std::thread([&]()
    {
        while(solve_running)
        {
            if(calc_running == false)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            else
            {
                for(size_t k = 0; k < count; ++k)
                {
                    size_t i = i_start + k, j = j_start - k;
                    const double v_old = memory->v[i][j];
                    memory->v[i][j] = memory->operator()(i, j);
                    precision = std::max(precision, std::abs(v_old - memory->v[i][j]));
                }
                calc_running = false;
            }
        }
    });
};

SOR_COL::colored_thread::~colored_thread()
{
    solve_running = false;
    executer.join();
};

int numeric_method::SOR_COL::solve(const double prec, const int N_max)
{
    int N = 0;

    const int overclocking_count = std::min(n,m) - 3;
    const int stationary_count = std::max(n,m) - overclocking_count - 2;

    const bool i_bigger = n > m;
    colored_thread solvers[] = {this, this};

    for(;N < N_max;++N)
    {
        solvers[0].i_start = 1;
        solvers[0].j_start = 0;
        solvers[0].count = 0;

        solvers[1].i_start = 1;
        solvers[1].j_start = 0;
        solvers[1].count = 0;

        for(int k = 0; k < overclocking_count; ++k) //overclocking
        {
            ++solvers[0].j_start;
            if (k % 2 == 0)
            {
                ++solvers[0].count;
                ++solvers[1].i_start;
            }
            else
            {
                ++solvers[1].count;
                ++solvers[1].j_start;
            }

            int test1 = solvers[0].j_start - solvers[0].count;
            int test2 = solvers[1].j_start - solvers[1].count;

            //solvers[1].i_start = solvers[0].i_start + solvers[0].count;
            //solvers[1].j_start = solvers[0].j_start - solvers[0].count;


            solvers[0].calc_running = true;
            solvers[1].calc_running = true;

            while(solvers[0].calc_running || solvers[1].calc_running)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        if (i_bigger)
            for (size_t not_regular = 1; not_regular < stationary_count; ++not_regular)
            {
                ++solvers[0].i_start;
                ++solvers[1].i_start;

                solvers[0].calc_running = true;
                solvers[1].calc_running = true;

                while(solvers[0].calc_running || solvers[1].calc_running)
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        else
            for (size_t not_regular = 1; not_regular < stationary_count; ++not_regular)
            {
                ++solvers[0].j_start;
                ++solvers[1].j_start;

                solvers[0].calc_running = true;
                solvers[1].calc_running = true;

                while(solvers[0].calc_running || solvers[1].calc_running)
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

        for(size_t k = 0; k < overclocking_count; ++k) //underclocking
        {

            ++solvers[0].i_start;

            if (k % 2)
            {
                --solvers[0].count;
                ++solvers[1].j_start;
            }
            else
            {
                --solvers[1].count;
                ++solvers[1].i_start;
            }

            //solvers[1].i_start = solvers[0].i_start + solvers[0].count;
            //solvers[1].j_start = solvers[0].j_start - solvers[0].count;


            solvers[0].calc_running = true;
            solvers[1].calc_running = true;

            while(solvers[0].calc_running || solvers[1].calc_running)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        precision = std::max(solvers[0].precision, solvers[1].precision);
        if(precision < prec)
            break;
    }

    return N;
};

}
