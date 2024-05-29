#pragma once

#include "Matrix_solver.h"

namespace numeric_method
{
// Successive over-relaxation method
class MWR : public Matrix_solver
{
public:
    double w;                 ///< Relaxation parameter
public:
    MWR(const size_t _n, const size_t _m);
    MWR(const size_t _n, const size_t _m, numeric_method::test dummy);
    virtual double operator()(const size_t i, const size_t j) const  override;
    virtual ~MWR();

    int solve( const double precision, const int N_max) override;
protected:
    double w_a;                     ///< w / A, for faster calculation
    double x_step_2, y_step_2;      ///< Difference scheme step
    using Matrix_solver::solve;
};
};
