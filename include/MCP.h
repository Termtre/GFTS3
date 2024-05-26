#pragma once

#include "Matrix_solver.h"

namespace numeric_method
{
// Метод с чебышевским набором параметров
class MCP : public Matrix_solver
{
public:
    double K;                 // Парамерт для метода
public:
    MCP(const size_t _n, const size_t _m);
    MCP(const size_t _n, const size_t _m, numeric_method::test dummy);
    double operator()(const size_t i, const size_t j) const override;
    ~MCP() override;
private:
    double w_a;                     // ??
    const double x_step_2, y_step_2;// ??
};

}
