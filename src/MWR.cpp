#include <cmath>
#include <algorithm>

#include "../include/MWR.h"

Matrix_solver::Matrix_solver(const size_t _n, const size_t _m) :v(_n), n(_n), m(_m), x_step(1.f/n), y_step(1.f/m)
{
    for(auto& i : v)
        v.resize(_n);

    for(size_t i = 0; i < n + 1; ++i)
    {
        float v_new = x_step * i; v_new *= 1.0f - v_new; // x - x*x;
        v[i][0] = v_new;
        v[i][m] = v_new;
    };

    for(size_t j = 0; j < m + 1; ++j)
    {
        float v_new = std::sin(PI * y_step *j);
        v[0][j] = v_new;
        v[n][j] = v_new;
    };

    for(size_t i = 1; i < n; ++i)
    {
        for(size_t j = 1; j < m; ++j)
            v[i][j] = v[i][0]; // linear interpolation, can use v[0][j]!
    };
        //TODO check positive definite
};



MWR::MWR(const size_t _n, const size_t _m):
    Matrix_solver(_n,_m),
    w(findW()),
    x_step_2(1.0f / (x_step * x_step)), y_step_2(1.0f / (y_step * y_step))
    {
        if (w < 0.0f || w > 2.0f)
            //not good

        w_a = - w * (x_step_2 + y_step_2) / 2;
    };

float MWR::operator()(const size_t i, const size_t j) const
{
    float v_ret = f(i,j) - x_step_2 * (v[i + 1][j] + v[i - 1][j]) + y_step_2 * (v[i][j+1] + v[i][j-1]); // Maybe need change direction of operation
    v_ret *= w_a;
    v_ret += (1.0f - w) * v[i][j];
    return v_ret;
};

float MWR::findW() const
{
    float r = 0.0f;
    for(size_t i = 0; i < n + 1; ++i)
    {
        float r_i = 1.0f / v[i][i];
        float sum = 0;
        for(size_t j = 0; j < m + 1; ++j)
            if( i != j)
                sum += std::abs(v[i][j]);
        r = std::max(r, sum + r_i);
    }
    float inv_opt_w = 1.0f + std::sqrt((1.0f - r)*(1.0f + r));
    return 2.0f / inv_opt_w;
};


float solve(Matrix_solver & s, const float Emet, const size_t Nmax)
{
    float eps;
    Matrix &v = *s.get_v();
    for (size_t count = 0; count < Nmax; ++ count)
    {
        eps = 0;
        for(size_t i = 1; i < s.n; ++i)
            for(size_t j = 1; j < s.m; ++j)
            {
                const float v_old = v[i][j];
                v[i][j] = s(i,j);
                eps = std::max(eps,std::abs(v_old - v[i][j]));
            }
        if (eps < Emet)
            return eps;
    }
    return eps;
};
