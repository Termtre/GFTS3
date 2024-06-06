#pragma once

#include "Matrix_solver.h"

namespace numeric_method
{
// Метод минимальных невязок
class MMR : public Matrix_solver
{
public:
    double t_ = 2. / 17.;
    MMR(const size_t _n, const size_t _m);
    MMR(const size_t _n, const size_t _m, numeric_method::test dummy);
    double operator()(const size_t i, const size_t j) const override{return 0.0;};
    ~MMR() override;
    int solve(const double precision, const int N_max) override;
private:
    const double x_step_2, y_step_2;      ///< Difference scheme step

    Matrix residual;
    Matrix A_r;

private:
    void getResidual();
    void getAr();
};

}
