#include "../include/MWR.h"
#include <iostream>
namespace numeric_method
{

MWR::MWR(const size_t _n, const size_t _m):
    Matrix_solver(_n,_m),
    w(findW()),
    x_step_2( static_cast<double>(_n * _n)),
    y_step_2( static_cast<double>(_m * _m))
    {
        if (w < 0.0f || w > 2.0f){};
            //not good
        w_a = 0.5 * w / static_cast<double>(_n * _n + _m * _m);
    };

double MWR::operator()(const size_t i, const size_t j) const
{
    return (1.0 - w) * v[i][j] + w_a *
    (
        f(i,j) +
        x_step_2 * (v[i + 1][j] + v[i - 1][j]) +
        y_step_2 * (v[i][j + 1] + v[i][j - 1])
    );
};

constexpr double MWR::findW() const
{
    // double r = 0.0;
    // for(size_t i = 1; i < n; ++i)
    // {
    //     double sum = 0.0;
    //     for(size_t j = 1; j < m; ++j)
    //         if( i != j)
    //             sum += std::abs(v[i][j]);
    //     r = std::max(r, sum);
    // }
    //
    // double inv_opt_w = 1.0 + std::sqrt((1.0 + r)*(1.0 - r));

    //TODO eigenvalue = m, its weird

    return 1.3; // inv_opt_w;
};

}
