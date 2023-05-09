#include <functional>
#include <vector>
namespace SimAnneal {

class Anneal1D {
    class Impl;
    Impl* pimpl;
public:
    Anneal1D(
        std::function<double(double)> E,
        double T0, 
        double Tf, 
        double tau, 
        double x,
        double scale
    );
    void show_plot();
    double operator()();
    ~Anneal1D();
};

}
