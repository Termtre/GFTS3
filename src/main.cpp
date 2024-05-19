#include <iostream>
#include <vector>
#include <memory>

#include "../include/Task_manager.h"

int main(int argc, char ** argv)
{
    const size_t n = 50, m = 50;
    const int core = 4;

    Task_manager manager(core);

    manager.createTask(n,m,Numerical_method::MWR_MAIN);
    manager.createTask(n,m,Numerical_method::MWR_TEST);
    manager.createTask(n,m,Numerical_method::MWR_BIGGER);


    size_t count = 0, count_test = 0, count_bigger = 0;
    std::unique_ptr<numeric_method::Matrix_solver> sN = manager.returnTask(count, Numerical_method::MWR_MAIN);
    std::unique_ptr<numeric_method::Matrix_solver> sN_test = manager.returnTask(count_test, Numerical_method::MWR_TEST);
    std::unique_ptr<numeric_method::Matrix_solver> s2N = manager.returnTask(count_bigger, Numerical_method::MWR_BIGGER);



    std::cout << std::scientific;
    //std::cout << dynamic_cast<numeric_method::MWR *>(&(*sN))->w << std::endl; ///< How read variables from ingeritance
    numeric_method::Matrix err (n + 1, std::vector<double>(m + 1, 0.0));
    numeric_method::Matrix err_test (n + 1, std::vector<double>(m + 1, 0.0));
    for(size_t i = 0; i < n + 1; ++i)
        for(size_t j = 0; j < m + 1; ++j)
        {
            err[i][j] = sN->v[i][j] - s2N->v[2*i][2*j];
            const double tmp = std::sin(numeric_method::pi * i * j / (n * m));
            err_test[i][j] = sN_test->v[i][j] - std::exp(tmp * tmp);
        }


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

    std::cout /*<< *sN_test */<< count <<", " << count_bigger << ", "<< eps << std::endl;
    std::cout /*<< *sN */<< count_test << ", "<<eps_test << std::endl;
    return 0;
}
