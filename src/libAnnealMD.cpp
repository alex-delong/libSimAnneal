#include <libAnnealMD.hpp>
#include <functional>
#include <cmath>
#include <random>
#include <chrono>
#include <assert.h>
#include <iostream>
#include <matplot/matplot.h>
#include <matplot/core/figure_type.h>
#include <vector>
using namespace SimAnneal;

class AnnealMD::Impl {
    std::function<double(std::vector<double>)> E;
    double T0;
    double Tf;
    double tau;
    std::vector<double> x;
    double scale;
    std::default_random_engine generator;
    std::normal_distribution<double> norm_dist;
    std::vector<std::vector<double>> v_x;
    std::vector<double> v_i;
    static double P(double delta_E, double T) {
        return exp(-delta_E/T);
    }
    double T(unsigned i) const {
        return this->T0*exp(-double(i)/this->tau);
    }
    std::vector<double> metropolis(std::vector<double> x0, std::vector<double> x1, unsigned i) {
        double delta_E = this->E(x1) - this->E(x0);
        if (P(delta_E, T(i)) < this->norm_dist(this->generator) ) {
            return x0;
        } else {
            return x1;
        }
    }

public:
    Impl(
        std::function<double(std::vector<double>)> E,
        double T0,
        double Tf,
        double tau,
        std::vector<double> x,
        double scale
    ) : 
        E(E),
        T0(T0),
        Tf(Tf),
        tau(tau),
        x(x),
        scale(scale)
    {} 
    void show_plot() {
        auto f = matplot::figure(false);
        auto ax = matplot::gca();
        auto p = ax->plot(this->v_i, this->v_x);
        f->show();
    }
    std::vector<double> operator()() {
        std::vector<double> delta_x;
        std::vector<double> x_new;
        for (int i = 0; T(i) > this->Tf; i++) {
            for (int j = 0; j < this->x.size(); j++) {
                delta_x.push_back(this->norm_dist(this->generator)*T(i));
            }
            for (int k = 0; k < this->x.size(); k++) {
                x_new.push_back(this->x[i] + delta_x[i]);
            }
            this->x = this->metropolis(this->x, x_new, i);
            this->v_i.push_back(i);
            this->v_x.push_back(this->x);
        }
        return this->x;
    }

};

AnnealMD::AnnealMD(
    std::function<double(std::vector<double>)> E,
    double T0,
    double Tf,
    double tau,
    std::vector<double> x,
    double scale
) : 
    pimpl(new Impl(E, T0, Tf, tau, x, scale))
{}
std::vector<double> AnnealMD::operator()() {
    return (*this->pimpl)();
}
void AnnealMD::show_plot() {
    this->pimpl->show_plot();
}
AnnealMD::~AnnealMD() {
    delete this->pimpl;
}
