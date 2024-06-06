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
    1.9933
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
    const int diagonal_num = n + m - 1;
    for(;N < N_max;++N)
    {
        double eps = 0;
        printf("s: %d eps: %.12lf\n", N, eps);
        for(int idx = 1; idx < diagonal_num - 1; ++idx)
        {
            const size_t j0 = std::min(idx, static_cast<int>(n) - 1);
            const size_t i0 = std::max(1, idx - static_cast<int>(m) + 2);
            const size_t diagonal_elem = j0 - i0 + 1;
            //#pragma omp parallel for reduction(max:eps)
            for(size_t d = 0; d < diagonal_elem; ++d)
            {
                const size_t i = i0 + d, j = j0 - d;
                const double v_old = v[i][j];
                v[i][j] = operator()(i,j);
                const double eps_point = std::abs(v_old - v[i][j]);
                if(eps_point > eps)
                    eps = eps_point;
            }
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
