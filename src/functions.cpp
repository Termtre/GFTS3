#include "..\include\functions.h"
#include <cmath>

static const double PI = 3.14159265358979323846;

double U(double x, double y)
{
    return exp(sin(2. * PI * x * y));
}

double f(double x, double y)
{
    return pow(sin(PI * x * y), 2.);
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
