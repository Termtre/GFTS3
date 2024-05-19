#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <memory>

#include "../include/MWR.h"

struct ThreadInput
{
    const double Emet;
    const size_t Nmax;

    std::unique_ptr<numeric_method::Matrix_solver> s;
    size_t count = 0;

    ThreadInput(std::unique_ptr<numeric_method::Matrix_solver>&& _s)
    :Emet(0.000000005),
     Nmax(100000),
     s(std::move(_s))
    {};

    ThreadInput(const double _Emet, const size_t _Nmax, std::unique_ptr<numeric_method::Matrix_solver>&& _s)
    :Emet(_Emet),
    Nmax(_Nmax),
    s(std::move(_s))
    {};
};

void ThreadFunction (ThreadInput& p)
{
    p.count = numeric_method::solve( *(p.s), p.Emet, p.Nmax);
};

int main(int argc, char ** argv)
{
    const size_t n = 10, m = 10;
    const int core = 4;
    std::vector<ThreadInput> task;
    std::vector<std::thread> solvers(core);

    task.push_back(ThreadInput(std::make_unique<numeric_method::MWR>(n,m)));
    task.push_back(ThreadInput(std::make_unique<numeric_method::MWR>(n,m,numeric_method::test{}) ));
    task.push_back(ThreadInput(std::make_unique<numeric_method::MWR>(2*n,2*m)));

    for(size_t i = 0; i <= task.size()/core ; ++i)
    {
        int n = 0;
        for(size_t j = 0; j < core && i * core + j < task.size(); ++j, ++n)
            solvers[j] = std::thread(ThreadFunction,std::ref(task[i * core + j]));

        for(size_t j = 0; j < n; ++j)
            solvers[j].join();
    }

    numeric_method::Matrix err (n + 1, std::vector<double>(m + 1, 0.0));
    numeric_method::Matrix err_test (n + 1, std::vector<double>(m + 1, 0.0));
    for(size_t i = 1; i < n; ++i)
        for(size_t j = 1; j < m; ++j)
        {
            err[i][j] = task[0].s->v[i][j] - task[2].s->v[2*i][2*j]; // @warning
            const double tmp = std::sin(numeric_method::pi * i * j / (n * m));
            err_test[i][j] = task[1].s->v[i][j] - std::exp(tmp * tmp);
        }

    std::cout << std::scientific;

    double eps = 0.0;
    double eps_test = 0.0;
    for(size_t i = 0; i < n + 1; ++i)
    {
        for(size_t j = 0; j < m + 1; ++j)
        {
            eps = std::max(eps,err[i][j]);
            eps_test = std::max(eps_test,err_test[i][j]);
        }
    }
    std::cout << eps << '\t' << eps_test;
    return 0;
}
