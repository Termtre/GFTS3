#pragma once

#include "Matrix_solver.h"

namespace numeric_method
{
// Метод простой итерации
class MSI : public Matrix_solver
{
public:
    double tau2;                 // Парамерт для метода

public:
    MSI(const size_t _n, const size_t _m);
    MSI(const size_t _n, const size_t _m, numeric_method::test dummy);
    double operator()(const size_t i, const size_t j) const override;
    ~MSI() override;
private:
    double tau;                     // ??
    const double x_step_2, y_step_2;// ??
};

}
