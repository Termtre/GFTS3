#include "../include/MWR.h"
//#include <iostream>
namespace numeric_method
{

MWR::MWR(const size_t _n, const size_t _m)
    :Matrix_solver(_n,_m),
    w([&]()
    {
        //@see https://www.sciencedirect.com/science/article/pii/S0893965908001523
        const size_t N = (_n - 1) * (_m - 1) + 1;
        //return 2.0 / ( 1.0 + std::sin(pi / static_cast<double>(N)));
        return 1.993;
    }()),
    x_step_2( _n * _n ), y_step_2( _m * _m )
    {
        w_a = 0.5 * w / static_cast<double>(_n * _n + _m * _m);
    };


MWR::MWR(const size_t _n, const size_t _m, numeric_method::test dummy)
    :Matrix_solver(_n,_m, dummy),
    w([&]()
    {
        //@see https://www.sciencedirect.com/science/article/pii/S0893965908001523
        const size_t N = (_n - 1) * (_m - 1) + 1;
        //return 2.0 / ( 1.0 + std::sin(pi / static_cast<double>(N)));
        return 1.993344;
    }()),
    x_step_2( _n * _n ), y_step_2( _m * _m )
    {
        w_a = 0.5 * w / static_cast<double>(_n * _n + _m * _m);
    };

double MWR::operator()(const size_t i, const size_t j) const
{
    return (1.0 - w) * v[i][j] + w_a *
    (
        f[i][j] +
        x_step_2 * (v[i + 1][j] + v[i - 1][j]) +
        y_step_2 * (v[i][j + 1] + v[i][j - 1])
                                            );
}

MWR::~MWR()
{};

}
