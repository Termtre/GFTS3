#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../include/Task_manager.h"

#define core 6

void MainWindow::uiMWR()
{
    size_t n = rows - 1;
    size_t m = columns - 1;

    Task_manager manager(core);

    manager.createTask(n,m,Numerical_method::MWR_MAIN);
    manager.createTask(n,m,Numerical_method::MWR_TEST);
    manager.createTask(n,m,Numerical_method::MWR_BIGGER);


    size_t count = 0, count_test = 0, count_bigger = 0;
    std::unique_ptr<numeric_method::Matrix_solver> sN = manager.returnTask(count, Numerical_method::MWR_MAIN);
    std::unique_ptr<numeric_method::Matrix_solver> sN_test = manager.returnTask(count_test, Numerical_method::MWR_TEST);
    std::unique_ptr<numeric_method::Matrix_solver> s2N = manager.returnTask(count_bigger, Numerical_method::MWR_BIGGER);

    //std::cout << dynamic_cast<numeric_method::MWR *>(&(*sN))->w << std::endl; ///< How read variables from ingeritance

    double eps = 0.0;
    double eps_test = 0.0;

//    if (ui->comboBox_2->currentIndex())
//    {
//        for(size_t i = 0; i < n + 1; ++i)
//        {
//            for(size_t j = 0; j < m + 1; ++j)
//            {
//                err[i][j] = sN->v[i][j] - s2N->v[2*i][2*j];
//                const double tmp = std::sin(numeric_method::pi * i * j / (n * m));
//                err_test[i][j] = sN_test->v[i][j] - std::exp(tmp * tmp);
//            }
//        }
//    }
//    else
//    {
//        for(size_t i = 0; i < n + 1; ++i)
//        {
//            for(size_t j = 0; j < m + 1; ++j)
//            {
//                err[i][j] = sN->v[i][j] - s2N->v[2*i][2*j];
//                const double tmp = std::sin(numeric_method::pi * i * j / (n * m));
//                err_test[i][j] = sN_test->v[i][j] - std::exp(tmp * tmp);
//            }
//        }
//    }

//    for(size_t i = 0; i < n + 1; ++i)
//    {
//        for(size_t j = 0; j < m + 1; ++j)
//        {
//            eps = std::max(eps,err[i][j]);
//            eps_test = std::max(eps_test,err_test[i][j]);
//        }
//    }
}
