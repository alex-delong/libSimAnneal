#include <functional>
#include <vector>
namespace SimAnneal {

class AnnealMD {
    class Impl;
    Impl* pimpl;
public:
    AnnealMD(
        std::function<double(std::vector<double>)> E,
        double T0, 
        double Tf, 
        double tau, 
        std::vector<double> x,
        double scale
    );
    void show_plot();
    std::vector<double> operator()();
    ~AnnealMD();
};

}
