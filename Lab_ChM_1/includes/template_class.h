#pragma once
#include <iostream>
#include <cmath>

using namespace std;

class Ax_b
{
public:
	Ax_b(double** in_Matrix, int in_n, double* in_b, double in_t_, double in_epsilon, int in_N);
	~Ax_b();

	double** Matrix_A;
	int n;                  // на самом деле n*m  x(x1,x2,...,xn) y(y1,y2,...,ym)
							// n стоит так как я делал для общего случая
							// Надеюсь на matrix solver Вадима
	double* b;
	double t_;              // 1/(Mmin + Mmax) из оценок собств. чисел    (Вадим, мне жаль...   (TT_TT  )  )

	double epsilon;         // Точность       Метод проходит итерации пока не достигнет этой точности (или после N шагов)
	int N;                  // Критерий остановки - max кол-во ходов    (epsilon = 0 , чтобы метод работал N итераций)

	//------------------------------------------------
	//     Для таблиц и др. (можно убрать, если не будем хранить)

	double* r; // Невязка                     // r [шаг метода] [эл-т вектора]
	double* x_0; // Приближения               // x0 [шаг метода] [эл-т вектора]

	double norma_r;
	double my_epsilon;// текущая точность

	void S_I_M_no_input(); // x0(0,0,...,0,1)
	void S_I_M(double* x0);  // Метод Простых Итераций

	void M_R_M_no_input();
	void M_R_M(double* x0); // Метод Минимальных Невязок



private:

	
	double norma(double* x);  //  || x ||   - норма x
	double* a_minus_b(double* a, double* b); //vectors a,b    return a-b
	double* var_x(double var, double* x); // число * вектор
	double scalar_multiplication(double* a, double* b); //скалярное произведение (a,b)
	double* A_x(double* x); // Matrix * Vector

	

	

};

