#include <libAnneal1D.hpp>
#include <libAnnealMD.hpp>
#include <cmath>
#include <functional>
#include <iostream>
#include <matplot/matplot.h>
#include <vector>

constexpr static const double pi = 3.1415926535f;
constexpr static const double T0 = 10.0f;
constexpr static const double Tf = 0.01f;
constexpr static const double tau = 2000.0f;
constexpr static const double x_init_1D = 5.0;
static const std::vector<double> x_init_MD = {5.0, 5.0};
constexpr static const double scale = 1.0;

void Anneal_1D() {
    using namespace SimAnneal;
    std::function<double(double)> energy_fn_1D = [](double x) constexpr -> double {
        return pow(x, 2.0) - 2.0*cos(2.0*pi*x);
    };
    std::vector<double> x = matplot::linspace(-5.0, 5.0, 100);
    std::vector<double> y = matplot::transform(x, energy_fn_1D);
    matplot::plot(x, y);
    matplot::show();
    Anneal1D one_dim_anneal(energy_fn_1D, T0, Tf, tau, x_init_1D, scale);
    double result_1D = one_dim_anneal();
    std::cout << result_1D << std::endl;
    one_dim_anneal.show_plot();
}

void Anneal_MD() {
    using namespace SimAnneal;
    std::function<double(std::vector<double>)> energy_fn_MD = [](std::vector<double> v) -> double {
        double x = v[0];
        double y = v[1];
        double r = sqrt(pow(x, 2.0) + pow(y, 2.0));
        return pow(r, 2.0) - 2.0*cos(2.0*pi*r);
    }; 
    AnnealMD m_dim_anneal(energy_fn_MD, T0, Tf, tau, x_init_MD, scale);
    std::vector<double> result_MD = m_dim_anneal();
    std::cout << result_MD[0] << " " << result_MD[1] << std::endl;
    m_dim_anneal.show_plot();
}

int main() {
    Anneal_1D();
    Anneal_MD();
}
