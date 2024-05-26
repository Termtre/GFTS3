#pragma once

#include "Matrix_solver.h"

namespace numeric_method
{
// Метод сопряженных градиентов
class MCG : public Matrix_solver
{
public:
    double K;                 // Парамерт для метода
public:
    MCG(const size_t _n, const size_t _m);
    MCG(const size_t _n, const size_t _m, numeric_method::test dummy);
    double operator()(const size_t i, const size_t j) const override;
    ~MCG() override;
private:
    double w_a;                     // ??
    const double x_step_2, y_step_2;// ??
};

}
