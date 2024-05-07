#include <vector>
#include <functional>
#include <cmath>
#include <ostream>

namespace numeric_method
{

using Matrix = std::vector<std::vector<double>>;

constexpr double pi = std::atan(1.0) * 4;

class Matrix_solver
{
public:
    Matrix_solver(const size_t _n, const size_t _m);

    inline Matrix* get_v(){return &v;};

    inline const Matrix* get_v() const {return &v;};

    virtual double operator()(const size_t i, const size_t j) const = 0;
public:
    const size_t n, m;
protected:
    Matrix v;

    const double x_step, y_step;

    std::function<double(const size_t, const size_t)> f = [this](const size_t i,const size_t j) // or just to cache in array
    {
        double ret = std::sin(pi * i * x_step * j * y_step );
        return ret * ret;
    };
};

std::ostream & operator<<(std::ostream& out, const Matrix_solver& s);

void solve(Matrix_solver & s);
}
