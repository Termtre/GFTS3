#include "../include/MWR.h"
namespace numeric_method
{

MWR::MWR(const size_t _n, const size_t _m)
    :Matrix_solver(_n,_m),
    w(/*[&]()
    {
        //@see https://www.sciencedirect.com/science/article/pii/S0893965908001523
        const size_t N = (_n - 1) * (_m - 1) + 1;
<<<<<<< HEAD
        return 2.0 / ( 1.0 + std::sin(pi / static_cast<double>(N)));
    }()*/
    1.9944
    ),
    x_step_2( _n * _n ), y_step_2( _m * _m )
    {
        w_a = 0.5 * w / static_cast<double>(_n * _n + _m * _m);
    };


MWR::MWR(const size_t _n, const size_t _m, numeric_method::test dummy)
    :Matrix_solver(_n,_m, dummy),
    w(/*[&]()
    {
        //@see https://www.sciencedirect.com/science/article/pii/S0893965908001523
        const size_t N = (_n - 1) * (_m - 1) + 1;
<<<<<<< HEAD
        return 2.0 / ( 1.0 + std::sin(pi / static_cast<double>(N)));
    }()*/
    1.993
    ),

    x_step_2( _n * _n ), y_step_2( _m * _m )
    {
        w_a = 0.5 * w / static_cast<double>(_n * _n + _m * _m);
    };

double MWR::operator()(const size_t i, const size_t j) const
{
    return std::fma(1.0 - w, v[i][j], w_a *
                                (f[i][j] +
                                x_step_2 * (v[i + 1][j] + v[i - 1][j]) +
                                y_step_2 * (v[i][j + 1] + v[i][j - 1])
                                ));
};

int MWR::solve( const double precision, const int N_max)
{
    int N = 0;
    for(;N < N_max;++N)
    {
        double eps = 0;
        for(size_t i = 1; i < n; ++i)
            for(size_t j = 1; j < m; ++j)
            {
                const double v_old = v[i][j];
                v[i][j] = this->operator()(i,j);
                eps = std::max(eps, std::abs(v_old - v[i][j]));
            }
        this->precision = eps;
        if(this->precision < precision)
            break;
    }
    return N;
};

MWR::~MWR()
{};

}
