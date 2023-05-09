#include <libSimAnneal.hpp>
#include <cmath>
#include <functional>
#include <iostream>
#include <matplot/matplot.h>
#include <vector>

constexpr static const double pi = 3.1415926535f;
constexpr static const double T0 = 10.0f;
constexpr static const double Tf = 0.01f;
constexpr static const double tau = 2000.0f;
constexpr static const double x_init = 5.0;
constexpr static const double scale = 1.0;

int main() {
    using namespace SimAnneal;
    std::function<double(double)> energy_fn = [](double x) -> double {
        return powf(x, 2.0) - 2.0*cosf(2.0*pi*x);
    };
    std::vector<double> x = matplot::linspace(-5.0, 5.0, 100);
    std::vector<double> y = matplot::transform(x, energy_fn);
    matplot::plot(x, y);
    matplot::show();
    Anneal1D one_dim_anneal(energy_fn, T0, Tf, tau, x_init, scale);
    double result = one_dim_anneal();
    std::cout << result << std::endl;
    one_dim_anneal.show_plot();

}
