#pragma once

#include "Matrix_solver.h"

namespace numeric_method
{
// Successive over-relaxation method
class MWR : public Matrix_solver
{
public:
    const double w;                 ///< Relaxation parameter
public:
    MWR(const size_t _n, const size_t _m);
    MWR(const size_t _n, const size_t _m, numeric_method::test dummy);
    double operator()(const size_t i, const size_t j) const override;
    virtual ~MWR() override;
private:
    double w_a;                     ///< w / A, for faster calculation
    const double x_step_2, y_step_2;///< Difference scheme step
};

}
