#include"../includes/template_class.h"


void Ax_b::S_I_M_no_input() // x0 - по стандарту (0,0,...,0,1)
{
	double* x0 = new double[n];

	for (int i = 0; i < n - 1; i++)
	{
		x0[i] = 0;
	}
	x0[n - 1] = 1;

	S_I_M(x0);
}

void Ax_b::S_I_M(double* x0) // Метод Простых Итераций
{



	double* x_prev = x0;
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
	cout << endl << "-----------------------------------" << endl << endl;
}