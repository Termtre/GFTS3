#include "../include/SOR_COL.h"

namespace numeric_method
{

SOR_COL::SOR_COL(const size_t _n, const size_t _m)
: MWR(_n, _m)
{
for(size_t i = 1; i < _n; ++i)
    for(size_t j = 1; j < _m; ++j)
        Towers[(i + j) % 4].board.push_back(std::pair<const size_t, const size_t>(i,j));
};

SOR_COL::SOR_COL(const size_t _n, const size_t _m, numeric_method::test dummy)
: MWR(_n, _m, dummy)
{
for(size_t i = 1; i < _n; ++i)
    for(size_t j = 1; j < _m; ++j)
        Towers[(i + j) % 4].board.push_back(std::pair<const size_t, const size_t>(i,j));
};

void SOR_COL::run()
{
    Towers[0].execute(*this);
    Towers[1].execute(*this);
    Towers[2].execute(*this);
    Towers[3].execute(*this);
};

void SOR_COL::join()
{
    Towers[0].executer.join();
    Towers[1].executer.join();
    Towers[2].executer.join();
    Towers[3].executer.join();
    eps = std::max({Towers[0].precision,
                     Towers[1].precision,
                     Towers[2].precision,
                     Towers[3].precision
    });
};
void SOR_COL::colored_thread::execute(Matrix_solver& MS)
{
    executer = std::thread([&](){
        for(auto& i: board)
        {
            const double v_old = MS.v[i.first][i.second];
            MS.v[i.first][i.second] = MS.operator()(i.first, i.second);
            precision = std::max(precision, std::abs(v_old - MS.v[i.first][i.second]));
        }
    });
};


int solve(SOR_COL & s, const double precision, const int N_max)
{
    int N = 0;
    for(;N < N_max;++N)
    {
        s.run();
        s.join();

        if(s.eps < precision)
            break;
    }

    return N;
};

};
