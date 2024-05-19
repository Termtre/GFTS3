#include "..\include\functions.h"
#include <cmath>

static const double PI = 3.14159265358979323846;

double U(double x, double y)
{
    double temp = sin(PI * x * y);
    return exp(temp * temp);
}

double f(double x, double y)
{
    double temp = sin(PI * x * y);
    return temp * temp;
}

double mu1(double y)
{
    return sin(PI * y);
}

double mu2(double y)
{
    return sin(PI * y);
}

double mu3(double x)
{
    return x - x * x;
}

double mu4(double x)
{
    return x - x * x;
}
