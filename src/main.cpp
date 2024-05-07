#include <iostream>
#include <thread>

#include "../include/MWR.h"

struct ThreadInput
{
    numeric_method::Matrix_solver& sN;
    numeric_method::Matrix_solver& s2N;
    const size_t n, m;
    const size_t Nmax;
    const double Emet;

    size_t count;
    double eps;

    double accuracy() const
    {
        const numeric_method::Matrix& vN  =  *sN.get_v();
        const numeric_method::Matrix& v2N = *s2N.get_v();

        double eps = 0;
        for(size_t i = 1; i < sN.n; ++i)
            for(size_t j = 1; j < sN.m; ++j)
                eps = std::max(eps, std::abs(vN[i][j] - v2N[2*i][2*j]));
        return eps;
    };
};

void threadFunction (ThreadInput& param)
{
    double eps = param.Emet + 1.0;
    size_t count = 0;
    for(; eps > param.Emet && count < param.Nmax; eps = param.accuracy(), ++count)
    {
        numeric_method::solve(param.sN);
        numeric_method::solve(param.s2N);
    }
    param.eps = eps;
    param.count = count;
};

int main(int argc, char ** argv)
{
    const size_t n = 80, m = 80;
    const double Emet = 0.0000005;
    const size_t Nmax = 10000;

    numeric_method::MWR sN(n,m);
    numeric_method::MWR s2N(2*n,2*m);

    ThreadInput MWR_param{.sN = sN, .s2N = s2N, .n = n, .m = m, .Nmax = Nmax, .Emet = Emet};

    std::thread MWR_method(threadFunction, std::ref(MWR_param));
    //TODO add own method to another thread

    MWR_method.join();

    std::cout << std::scientific;

    std::cout << MWR_param.count << '\n' << MWR_param.eps;
    return 0;
}
