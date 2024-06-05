#pragma once

#include <vector>
#include <functional>
#include <cmath>
#include <ostream>

namespace numeric_method
{

using Matrix = std::vector<std::vector<double>>;

struct test{};

//@note 16 number, long double == double wtf (https://onlinegdb.com)
const double pi = std::atan(1.0) * 4;

// abstract numerical method class
class Matrix_solver
{
public:
    Matrix_solver(const size_t _n, const size_t _m);
    Matrix_solver(const size_t _n, const size_t _m, test dummy);
    /**
     * Method of calculating next approcimaiton at point
     *
     * @param i, j point of grid
     * @return value in v[i][j] for next step approcimaiton
     */
    virtual double operator()(const size_t i, const size_t j) const = 0;

    virtual ~Matrix_solver();

    /**
     * @return calculated value of max|v - f| for all i,j
     */
    double calculate_residual() const;

    /**
    * calculate approcimaitons until precision or max number of iteration is reached
    *
    * @param precision Stop condition, difference between last and pre-last approcimaiton
    * @param N_max Stop condition, max number of iteration
    *
    * @return number of iteration
    */
    virtual int solve(const double precision, const int N_max);

public:
    const size_t n, m;          ///< Numbers of grid splits
    Matrix v;                   ///< Container for numerical method solve
    double precision;           ///< precision
    const Matrix f;             ///< Right part of Dirichlet problem
};

std::ostream & operator<<(std::ostream& out, const Matrix_solver& s);
}
