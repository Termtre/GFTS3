#include "Matrix_solver.h"

namespace numeric_method
{

class MWR : public Matrix_solver
{
public:
    MWR(const size_t _n, const size_t _m);

    double operator()(const size_t i, const size_t j) const;
private:
    constexpr double findW() const;
private:
    const double w;
    double w_a;
    const double x_step_2, y_step_2;
};

}
