#include"includes/template_class.h"

int main()
{


	double** Matrix = new double* [3];
	for (int i = 0; i < 3; i++)
	{
		Matrix[i] = new double[3];
	}

	Matrix[0][0] = 3.; Matrix[0][1] = -0.8; Matrix[0][2] = 0.2; 
	Matrix[1][0] = -0.8; Matrix[1][1] = 9.; Matrix[1][2] = 1.8; 
	Matrix[2][0] = 0.2; Matrix[2][1] = 1.8; Matrix[2][2] = 13.; 

	int n = 3; int m = 3;
	double b[3];
	b[0] = 3.2;
	b[1] = 1.;
	b[2] = 13.2;

	double x0[3]; // начальное приближение
	x0[0] = 0.;
	x0[1] = 1.;
	x0[2] = 0.;

	double t_ = 2. / 17.;
	
	double epsilon=0.01;  // Требуемая точность (не выше)
	int N=100;  // критерий остановки по шагам

	Ax_b Axb1(Matrix, n, b, t_, epsilon, N) ;
	Ax_b Axb2(Matrix, n, b, t_, epsilon, N);


	Axb1.S_I_M(x0); // Метод Простых Итераций
	Axb2.M_R_M(x0); // Метод Минимальных Невязок


	//Axb1.S_I_M_no_input();
	//cout << endl << endl << endl;   // Тоже работает
	//Axb2.M_R_M_no_input();
}