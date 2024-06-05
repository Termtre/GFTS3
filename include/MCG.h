#pragma once

#include "Matrix_solver.h"

namespace numeric_method
{
// Метод сопряженных градиентов
class MCG : public Matrix_solver
{
public:
    MCG(const size_t _n, const size_t _m);
    MCG(const size_t _n, const size_t _m, numeric_method::test dummy);
    double operator()(const size_t i, const size_t j) const override{return 0.0;};
    ~MCG() override;
    int solve(const double precision, const int N_max) override;
private:
    const double x_step_2, y_step_2;      ///< Difference scheme step

    Matrix grad;
    Matrix dir;
    Matrix Adir;
private:
    void comp_grad();
};

}
