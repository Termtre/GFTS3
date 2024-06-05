#include "../include/MCG.h"

#include <numeric>
#include <omp.h>

namespace numeric_method
{

MCG::MCG(const size_t _n, const size_t _m)
    :Matrix_solver(_n,_m),
    grad(_n + 1, std::vector<double>(_m + 1)),
    dir(_n + 1, std::vector<double>(_m + 1)),
    Adir(_n + 1, std::vector<double>(_m + 1)),
    x_step_2( _n * _n ), y_step_2( _m * _m )
{
    comp_grad();
    dir = grad;
};


MCG::MCG(const size_t _n, const size_t _m, numeric_method::test dummy)
    :Matrix_solver(_n,_m, dummy),
    grad(_n + 1, std::vector<double>(_m + 1)),
    dir(_n + 1, std::vector<double>(_m + 1)),
    Adir(_n + 1, std::vector<double>(_m + 1)),
    x_step_2( _n * _n ), y_step_2( _m * _m )
{
    comp_grad();
    dir = grad;
};

MCG::~MCG()
    {};

void MCG::comp_grad()
{
    //#pragma omp parallel
    for (size_t i = 1; i < n; ++i)
        for(size_t j = 1; j < n; ++j)
            grad[i][j] = -f[i][j] -  (
                         (v[i + 1][j] - 2 * v[i][j] + v[i - 1][j]) * x_step_2 +
                         (v[i][j + 1] - 2 * v[i][j] + v[i][j - 1]) * y_step_2
                         );

    // for (size_t i = 1; i < n; ++i)
    // {
    //     grad[i][    1] += v[i][0] * y_step_2;
    //     grad[i][m - 1] += v[i][m] * y_step_2;
    // }

    // for (size_t j = 1; j < m; ++j)
    // {
    //     grad[    1][j] += v[0][j] * x_step_2;
    //     grad[n - 1][j] += v[n][j] * x_step_2;
    // }
};

int MCG::solve( const double precision, const int N_max)
{
    auto inner_prod = [](const Matrix& m1, const Matrix& m2)
    {
        return std::inner_product(m1.begin(), m1.end(), m2.begin(), 0.0,std::plus<>(), [](const std::vector<double>& g1,const std::vector<double>& g2)
        {
            return std::inner_product(g1.begin(), g1.end(), g2.begin(), 0.0);
        });
    };

    int N = 0;
    double eps = 0.0;
    double gradSquare = inner_prod(grad,grad);

    for(;N < N_max;++N)
    {
        eps = 0.0;
        // if(N%20 == 19)
        // {
        //     comp_grad();
        //     dir = grad;
        // }
        //#pragma omp parallel
        for(size_t i = 1; i < n; ++i)
            for(size_t j = 1; j < m; ++j)
                Adir[i][j] = (dir[i + 1][j] - 2 * dir[i][j] + dir[i - 1][j]) * x_step_2 + (dir[i][j + 1] - 2 * dir[i][j] + dir[i][j - 1]) * y_step_2;

        const double alpha = gradSquare / inner_prod(Adir, dir);

        //#pragma omp parallel for reduction(max:eps)
        for (size_t i = 1; i < n; ++i)
        {
            for(size_t j = 1; j < m; ++j)
            {
                const double incr = alpha * dir[i][j];
                v[i][j] += incr;
                grad[i][j] -= alpha * Adir[i][j];
                if(incr > eps)
                    eps = incr;
            }
        }

        const double newGradSquare = inner_prod(grad,grad);
        const double beta = newGradSquare / gradSquare;

        gradSquare = newGradSquare;

        //#pragma omp parallel
        for (size_t i = 1; i < n; ++i)
            for(size_t j = 1; j < m; ++j)
                dir[i][j] = grad[i][j] + beta * dir[i][j];

        if(eps < precision)
            break;
    }
    this->precision = eps;
    return N;
};

}
