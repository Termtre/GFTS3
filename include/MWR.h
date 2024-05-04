#include <vector>
#include <functional>
using Matrix = std::vector<std::vector<float>>;

static constexpr const float PI = std::atan(1.0f) * 4;

class Matrix_solver
{
public:
    Matrix_solver(const size_t _n, const size_t _m);

    inline Matrix * get_v(){return &v;};

    virtual float operator()(const size_t i, const size_t j) const = 0;
public:
    const size_t n, m;
protected:
    Matrix v;

    const float x_step, y_step;

    std::function<float(const size_t, const size_t)> f = [this](const size_t i,const size_t j) // or just to cache in array
    {
        float ret = std::sin(PI * i * x_step * j * y_step );
        return ret * ret;
    };
};

class MWR : public Matrix_solver
{
public:
    MWR(const size_t _n, const size_t _m);

    float operator()(const size_t i, const size_t j) const;
private:
    float findW() const;
private:
    const float w;
    float w_a;
    const float x_step_2, y_step_2;
};
