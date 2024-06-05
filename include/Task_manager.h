#pragma once

#include <thread>
#include <memory>

#include "MWR.h"
#include "SOR_COL.h"
#include "MCG.h"
>>>>>>> fc2c9d1d2a0ea1d6b12180f9d3114452b258a67a

enum class Numerical_method
{
    // relaxation
    MWR_MAIN,
    MWR_TEST,
    MWR_BIGGER,

    SOR_MAIN,
    SOR_TEST,
    SOR_BIGGER,

    // simple iter
    MSI_MAIN,
    MSI_TEST,
    MSI_BIGGER,
    // minimum resud
    MMR_MAIN,
    MMR_TEST,
    MMR_BIGGER,
    // chebishev
    MCP_MAIN,
    MCP_TEST,
    MCP_BIGGER,
    // gradient
    MCG_MAIN,
    MCG_TEST,
    MCG_BIGGER,
>>>>>>> fc2c9d1d2a0ea1d6b12180f9d3114452b258a67a
    //TODO add another method enumeration
    NONE
};

// Handler for calculation threads
class Task_manager
{
public:
    /**
     * @param core number of core in system
     */
    Task_manager(int core = 2);

    /**
     * Method for push task in manager
     *
     * @param n,m Numbers of grid splits
     * @param method Type of using method, @see enum Numerical_method
     * @param Emet Stop condition, relate from solve
     * @param Nmax Stop condition, max number of iteration
     */
    void createTask(const size_t n, const size_t m, const Numerical_method method, const double Emet = 0.000000005, const int Nmax = 100000);

    /**
     * Method for get data from manager
     * @param count place to write number of iteration
     * @param method Type of using method, @see enum Numerical_method
     *
     * @return pointer to answer
     */
    std::unique_ptr<numeric_method::Matrix_solver> returnTask(size_t & count, const Numerical_method method);

    /**
     * Procedure of waiting solution for specific method
     */
    void wait_solution(const Numerical_method method) const;

    virtual ~Task_manager();
private:
    /**
     * Method for find free calculation thread
     *
     * @return number of free thread
     */
    size_t find_free();

private:

struct ThreadInput
{
    const double Emet;
    const size_t Nmax;

    std::unique_ptr<numeric_method::Matrix_solver> s;
    size_t count = 0;
    bool calculated = false;

    ThreadInput(const double _Emet, const size_t _Nmax, std::unique_ptr<numeric_method::Matrix_solver>&& _s)
    :Emet(_Emet),
    Nmax(_Nmax),
    s(std::move(_s))
    {};

    void operator()()
    {
        count = s->solve(Emet, Nmax);
        calculated = true;
    }
};

class Task_solver
{
public:
    Task_solver();

    ~Task_solver();
public:
    ThreadInput* task = nullptr;
private:
    std::thread solver;  ///< Calculation thread
};

private:
    std::vector<std::unique_ptr<ThreadInput>> task;
    std::vector<Task_solver> solvers;
};

