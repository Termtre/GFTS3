#include "../include/Matrix_solver.h"
#include <iostream>
namespace numeric_method
{


Matrix_solver::Matrix_solver(const size_t _n, const size_t _m)
    :n(_n), m(_m),
    precision(0.0),
    v(_n + 1, std::vector<double>(_m + 1)),
    f([&]()
    {
        Matrix ret(_n + 1, std::vector<double>(_m + 1));
        for(size_t i = 1; i < _n; ++i)
            for(size_t j = 1; j < _m; ++j)
            {
                const double tmp = std::sin(pi * i * j / ((_n + 1) * (_m + 1)));
                ret[i][j] = - tmp * tmp; // maybe need change sign @see positive definite
            }
        return ret;
    }()
    )
{
    for(size_t i = 0; i < n + 1; ++i)
    {
        const double v_new =  static_cast<double>(i * (_n - i)) / (_n * _n); //x - x*x
        v[i][0] = v_new;
        v[i][m] = v_new;
    };

    for(size_t j = 1; j < m; ++j)
    {
        const double v_new = std::sin(pi * j / m);
        v[0][j] = v_new;
        v[n][j] = v_new;
    };

    for(size_t i = 1; i < n; ++i)
    {
        for(size_t j = 1; j < m; ++j)
        {
            v[i][j] = v[0][j]; // linear interpolation along y, can use v[0][j]!
        }
    };


    //TODO check positive definite
};


Matrix_solver::Matrix_solver(const size_t _n, const size_t _m, test dummy)
    :n(_n), m(_m),
    precision(0.0),
    v(_n + 1, std::vector<double>(_m + 1)),
    f([&]()
    {
        auto esin =[&](double x)
        {
            const double tmp = std::sin(pi * x);
            return - std::exp(tmp * tmp);
        };
        Matrix ret(_n + 1, std::vector<double>(_m + 1, 0.0));
        const double h = 1.0 / _n;
        const double k = 1.0 / _m;
        for(size_t i = 1; i < _n; ++i)
        {
            const double x = static_cast<double> (i) / _n;
            for(size_t j = 1; j < _m; ++j)
            {
                const double y = static_cast<double> (j) / _m;
                //ret[i][j] += _n * _n * (esin( (x + h) * y) + esin( (x - h) * y) - 2 * esin(x * y)); // d2/dx^2
                //ret[i][j] += _m * _m * (esin( x * (y + k)) + esin( x * (y - k)) - 2 * esin(x * y)); // d2/dy^2
                ret[i][j] = esin(x * y) * pi * pi *
                            (x * x + y * y) * (2. - 4. * sin(pi * x * y) * sin(pi * x * y) + sin(2. * pi * x * y) * sin(2. * pi * x * y));
            }
        }
        return ret;
    }()
    )
{
    auto mu = [](const double x){               // exp(sin^2(pi*t))
        const double tmp = std::sin(pi * x);
        return std::exp(tmp*tmp);
    };

    for(size_t i = 0; i < n + 1; ++i)
    {
        const double v_new = static_cast<double> (i) / n;
        v[i][0] = 1.0;
        v[i][m] = mu(v_new);
    };

    for(size_t j = 1; j < m; ++j)
    {
        const double v_new = static_cast<double> (j) / m;
        v[0][j] = 1.0;
        v[n][j] = mu(v_new);
    };

    for(size_t i = 1; i < n; ++i)
    {
        for(size_t j = 1; j < m; ++j)

            v[i][j] = (v[0][j] * (n + 1  - i) + v[n][j] * i) / (n + 1); // linear interpolation along y

    };
}

Matrix_solver::~Matrix_solver()
{

};

double Matrix_solver::calculate_residual() const
{
    double eps = 0.0;
    for (size_t i = 1; i < n; ++i)
    {
        for(size_t j = 1; j < m; ++j)
            eps = std::max(eps,std::abs(
                (v[i + 1][j] - 2 * v[i][j] + v[i - 1][j]) * n * n +
                (v[i][j + 1] - 2 * v[i][j] + v[i][j - 1]) * m * m -
                f[i][j]
            ));
    }
    return eps;
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

int Matrix_solver::solve( const double precision, const int N_max)
{
    int N = 0;
    double eps = 0.;
    for(;N < N_max;++N)
    {
        std::cout << "s: " << N << " eps: " << eps << std::endl;
        eps = 0.0;
        for(size_t i = 1; i < n; ++i)
            for(size_t j = 1; j < m; ++j)
            {
                const double v_new = operator()(i,j);
                eps = std::max(eps, std::abs(v_new - v[i][j]));
                v[i][j] = v_new;
            }
        if(eps < precision)
            break;
    }
    this->precision = eps;
    return N;
};
}
