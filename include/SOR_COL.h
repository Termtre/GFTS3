#pragma once
#include <thread>
#include <utility>
#include <vector>

#include "MWR.h"

namespace numeric_method
{

class SOR_COL : public MWR
{
public:
    SOR_COL(const size_t _n, const size_t _m);
    SOR_COL(const size_t _n, const size_t _m, numeric_method::test dummy);
    virtual ~SOR_COL() override;

    void run();
    void join();
private:
    struct colored_thread
    {
        std::thread executer;
        std::vector<std::pair<const size_t, const size_t>> board;
        double precision = 0.0;
        void execute(Matrix_solver& MS);
    };
    colored_thread Towers[4];
};

int solve(SOR_COL & s, const double precision, const int N_max);

};
