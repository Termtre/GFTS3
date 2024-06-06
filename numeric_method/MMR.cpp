#include "../include/MMR.h"
#include <iostream>
#include <numeric>

namespace numeric_method
{


MMR::MMR(const size_t _n, const size_t _m)
    :Matrix_solver(_n,_m),
    residual(_n + 1, std::vector<double>(_m + 1)),
    A_r(_n + 1, std::vector<double>(_m + 1)),
    x_step_2( _n * _n ), y_step_2( _m * _m )
{};


MMR::MMR(const size_t _n, const size_t _m, numeric_method::test dummy)
    :Matrix_solver(_n,_m, dummy),
    residual(_n + 1, std::vector<double>(_m + 1)),
    A_r(_n + 1, std::vector<double>(_m + 1)),
    x_step_2( _n * _n ), y_step_2( _m * _m )
{};

MMR::~MMR()
{}

int MMR::solve(const double precision, const int N_max)
{
    auto inner_prod = [](const Matrix& m1, const Matrix& m2)
    {
        return std::inner_product(m1.begin(), m1.end(), m2.begin(), 0.0,std::plus<>(), [](const std::vector<double>& g1,const std::vector<double>& g2)
                                  {
                                      return std::inner_product(g1.begin(), g1.end(), g2.begin(), 0.0);
                                  });
    };

    int N = 0;
    double eps;

    for (; N <= N_max; N++)
    {
        eps = 0.;

        getResidual();
        getAr();

        t_ = inner_prod(A_r, residual) / inner_prod(A_r, A_r);

        for (size_t i = 1; i < n; ++i)
        {
            for(size_t j = 1; j < m; ++j)
            {
                const double incr = t_ * residual[i][j];
                v[i][j] -= incr;
                if(incr > eps)
                    eps = incr;
            }
        }

        if (eps < precision)
            break;

        getResidual();
    }

    this->precision = eps;

    return N;
}

void MMR::getResidual()
{
    for (size_t i = 1; i < n; ++i)
    {
        for(size_t j = 1; j < m; ++j)
           residual[i][j] = +((v[i + 1][j] - 2 * v[i][j] + v[i - 1][j]) * n * n +
                              (v[i][j + 1] - 2 * v[i][j] + v[i][j - 1]) * m * m) +
                      f[i][j];
    }

}

void MMR::getAr()
{
    for(size_t i = 1; i < n; ++i)
        for(size_t j = 1; j < m; ++j)
            A_r[i][j] = (residual[i + 1][j] - 2. * residual[i][j] + residual[i - 1][j]) * x_step_2 +
                         (residual[i][j + 1] - 2. * residual[i][j] + residual[i][j - 1]) * y_step_2;
}

}
