#include "../include/MSI.h"

numeric_method::MSI::MSI(const size_t _n, const size_t _m):
    Matrix_solver(_n,_m),
    tau([&]()
      {
          //@see https://www.sciencedirect.com/science/article/pii/S0893965908001523
          const size_t N = (_n - 1) * (_m - 1) + 1;
          return 2.0 / ( 1.0 + std::sin(pi / static_cast<double>(N)));
      }()),
    x_step_2( _n * _n ), y_step_2( _m * _m )
{
    tau2 = 0.5 * tau / static_cast<double>(_n * _n + _m * _m);
};
//{
    /*double* x_prev = x0;
    r = a_minus_b(A_x(x0), b);  // r = A*x0 - b  , где x0 - начальное приближение.   Далее x0 - приближение (step)

    cout << endl <<"//---[ Simple Iteration Method ]---//"<<endl<<endl << "step=0" << endl << "  >>| ||r(0)|| = " << norma(r) << endl << endl << endl;

    int step = 1;

    while (epsilon <= my_epsilon && step <= N)
    {
        x0 = a_minus_b(x0, var_x(t_, r));   // x0[step] = x0[step-1] - t_ * r[step-1]   , где x0 - приближение, t_ - постоянное число метода (по оценке собств. чисел), r - невязка

        r = a_minus_b(A_x(x0), b);        // r[step] = A*x0[step] - b



        norma_r = norma(r);
        my_epsilon = norma(a_minus_b(x0, x_prev));

        cout << endl << "step=" << step << endl << "  >>| ||r(" << step << ")|| = " << norma_r << endl << "  >>| epsilon(" << step << ") = " << my_epsilon << endl << endl << endl;

        x_prev = x0;
        step++;
    }
    cout << "-[ solution ]-" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "x0[" << i << "] = " << x0[i] << endl;
    }
    cout << endl << "-----------------------------------" << endl << endl;*/
//}

numeric_method::MSI::MSI(const size_t _n, const size_t _m, test dummy):
    Matrix_solver(_n,_m, dummy),
    tau([&]()
      {
          //@see https://www.sciencedirect.com/science/article/pii/S0893965908001523
          const size_t N = (_n - 1) * (_m - 1) + 1;
          return 2.0 / ( 1.0 + std::sin(pi / static_cast<double>(N)));
      }()),
    x_step_2( _n * _n ), y_step_2( _m * _m )
{
    tau2 = 0.5 * tau / static_cast<double>(_n * _n + _m * _m);
};

double numeric_method::MSI::operator()(const size_t i, const size_t j) const
{
    return (1.0 - tau) * v[i][j] + tau2 *
                                     (
                                         f[i][j] +
                                         x_step_2 * (v[i + 1][j] + v[i - 1][j]) +
                                         y_step_2 * (v[i][j + 1] + v[i][j - 1])
                                         );
}

numeric_method::MSI::~MSI()
{}
