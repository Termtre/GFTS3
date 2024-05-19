#include "../include/Matrix_solver.h"
#include <iostream>
namespace numeric_method
{

Matrix_solver::Matrix_solver(const size_t _n, const size_t _m)
    :n(_n), m(_m),
    v(_n + 1, std::vector<double>(_m + 1)),
    f([&]()
    {
        Matrix ret(_n + 1, std::vector<double>(_m + 1));
        for(size_t i = 1; i < _n; ++i)
            for(size_t j = 1; j < _m; ++j)
            {
                const double tmp = std::sin(pi * i * j / ((_n + 1) * (_m + 1)));
                ret[i][j] = tmp * tmp;
            }
        return ret;
    }()
    )
{
    for(size_t i = 1; i < n; ++i)
    {
        const double v_new =  static_cast<double>(i * (_n - i)) / (_n * _n); //x - x*x
        v[i][0] = v_new;
        v[i][m] = v_new;
    };

    for(size_t j = 1; j < m; ++j) // @warning v[0][m],v[n][m] not use but can be trash
    {
        const double v_new = std::sin(pi * j / m);
        v[0][j] = v_new;
        v[n][j] = v_new;
    };

    for(size_t i = 1; i < n; ++i)
    {
        for(size_t j = 1; j < m; ++j)
            v[i][j] = v[i][0]; // linear interpolation to y, can use v[0][j]!
    };

    //TODO check positive definite
};


Matrix_solver::Matrix_solver(const size_t _n, const size_t _m, test dummy)
    :n(_n), m(_m),
    v(_n + 1, std::vector<double>(_m + 1)),
    f([&]()
    {
        Matrix ret(_n + 1, std::vector<double>(_m + 1));
        for(size_t i = 1; i < _n; ++i)
            for(size_t j = 1; j < _m; ++j)
            {
                double tmp = std::sin(pi * i * j / (_n * _m));
                tmp *= tmp;
                const double norm = static_cast<double>(i * i) / (_n * _n) + static_cast<double>(j * j) / ( _m * _m);
                ret[i][j] = - 2 * std::exp(tmp) * norm * ( 2 * tmp * tmp - 1);
            }
        return ret;
    }()
    )
{
    auto mu = [](const double x){               // exp(sin^2(pi*t))
        const double tmp = std::sin(pi * x);
        return std::exp(tmp*tmp);
    };

    for(size_t i = 1; i < n; ++i)
    {
        v[i][0] = 1.0;
        v[i][m] = mu(static_cast<double>(i) / n);
    };

    for(size_t j = 1; j < m; ++j)
    {
        v[0][j] = 1.0;
        v[n][j] = mu(static_cast<double>(j) / m);
    };

    for(size_t i = 1; i < n; ++i)
    {
        for(size_t j = 1; j < m; ++j)
            v[i][j] = v[i][0];
    };
};

std::ostream& operator<<(std::ostream& out, const Matrix_solver& s)
{
    for(size_t i = 0; i < s.n + 1; ++i)
    {
        for(size_t j = 0; j < s.m + 1; ++j)
            out << s.v[i][j] << '\t';
        out << '\n';
    };
    out << '\n';
    return out;
}

int solve(Matrix_solver & s, const double precision, const int N_max)
{
    int N = 0;
    for(;N < N_max;++N)
    {
        double eps = 0;
        for(size_t i = 1; i < s.n; ++i)
            for(size_t j = 1; j < s.m; ++j)
            {
                const double v_new = s(i,j);
                eps = std::max(eps, std::abs(v_new - s.v[i][j]));
                s.v[i][j] = v_new;
            }

        if(eps < precision)
            break;
    }

    return N;
};
}
