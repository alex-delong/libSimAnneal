#include <functional>
#include <vector>
namespace SimAnneal {
    class Anneal {
        class Impl;
        Impl* pimpl;
    public:
        Anneal(
            std::function<double(double)> E,
            double T0, 
            double Tf, 
            double tau, 
            double x,
            double scale
        );
        void show_plot();
        double operator()();
        ~Anneal();
    };
}
