#include "../include/Matrix_solver.h"

namespace numeric_method
{

Matrix_solver::Matrix_solver(const size_t _n, const size_t _m) :v(_n + 1), n(_n), m(_m), x_step(1.0/_n), y_step(1.0/_m)
{
    for(auto& i : v)
        i.resize(_m + 1);
    for(size_t i = 0; i < n + 1; ++i)
    {
        double v_new = x_step * i; v_new *= 1.0 - v_new; // x - x*x;
        v[i][0] = v_new;
        v[i][m] = v_new;
    };
    for(size_t j = 0; j < m; ++j)
    {
        double v_new = std::sin(pi * y_step *j);
        v[0][j] = v_new;
        v[n][j] = v_new;
    };
    v[0][m] = 0;
    v[n][m] = 0;

    for(size_t i = 1; i < n; ++i)
    {
        for(size_t j = 1; j < m; ++j)
            v[i][j] = v[i][0]; // linear interpolation, can use v[0][j]!
    };

    //TODO check positive definite
};

std::ostream& operator<<(std::ostream& out, const Matrix_solver& s)
{
    const Matrix v= *s.get_v();
    for(size_t i = 0; i < s.n + 1; ++i)
    {
        for(size_t j = 0; j < s.m + 1; ++j)
            out << v[i][j] << '\t';
        out << '\n';
    };
    out << '\n';
    return out;
}

void solve(Matrix_solver & s)
{
    Matrix& v = *s.get_v();
    for(size_t i = 1; i < s.n; ++i)
        for(size_t j = 1; j < s.m; ++j)
            v[i][j] = s(i,j);
};
}
