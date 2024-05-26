#pragma once

#include "Matrix_solver.h"

namespace numeric_method
{
// Метод минимальных невязок
class MMR : public Matrix_solver
{
public:
    double tau;                 // Парамерт для метода
public:
    MMR(const size_t _n, const size_t _m);
    MMR(const size_t _n, const size_t _m, numeric_method::test dummy);
    double operator()(const size_t i, const size_t j) const override;
    ~MMR() override;
private:
    double w_a;                     // ??
    const double x_step_2, y_step_2;// ??
};

}
