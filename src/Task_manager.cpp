#include "../include/Task_manager.h"

bool program_running = true;

Task_manager::Task_solver::Task_solver()
{
    solver = std::thread ([&]()
    {
        while(program_running)
            {
            if(task == nullptr)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            else
            {
                (*task)();
                task = nullptr;
            }
        }
    });
}
Task_manager::Task_solver::~Task_solver()
{
    solver.join();
};

Task_manager::Task_manager(const int core)
:solvers(core),
task(static_cast<size_t>(Numerical_method::NONE))
{};

void Task_manager::createTask(const size_t n, const size_t m, const Numerical_method method, const double Emet, const int Nmax)
{
    size_t i = static_cast<size_t>(method);
    switch(method)
    {
        case Numerical_method::MWR_MAIN:
            task[i] = std::make_unique<ThreadInput>(Emet, Nmax, std::make_unique<numeric_method::MWR>(n, m));
            break;
        case Numerical_method::MWR_BIGGER:
            task[i] = std::make_unique<ThreadInput>(Emet, Nmax, std::make_unique<numeric_method::MWR>(2 * n, 2 * m));
            break;
        case Numerical_method::MWR_TEST:
            task[i] = std::make_unique<ThreadInput>(Emet, Nmax, std::make_unique<numeric_method::MWR>(n, m, numeric_method::test{}));
            break;
        default:
            return;
            //TODO add new case with method
    }

    solvers[find_free()].task = &(*task[i]);
};

void Task_manager::wait_solution(const Numerical_method method) const
{
    size_t i = static_cast<size_t>(method);
    while(! (task[i]->calculated))
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
};

std::unique_ptr<numeric_method::Matrix_solver> Task_manager::returnTask(size_t& count, const Numerical_method method)
{
    size_t i = static_cast<size_t>(method);
    wait_solution(method);
    count = task[i]->count;
    return std::move(task[i]->s);
};

Task_manager::~Task_manager()
{
    program_running = false;
};

size_t Task_manager::find_free()
{
    size_t i = 0;
    while(true)
    {
        if (i == solvers.size()) i = 0;

        if(solvers[i].task != nullptr)
            ++i;
        else
            return i;
        std::this_thread::sleep_for(std::chrono::milliseconds(solvers.size() * 10));
    }
};
