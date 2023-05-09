#include <libSimAnneal.hpp>
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

double P(double delta_E, double T) {
    return exp(-delta_E/T);
}

class Anneal::Impl {
    std::function<double(double)> E;
    double T0;
    double Tf;
    double tau;
    double x;
    double scale;
    std::default_random_engine generator;
    std::normal_distribution<double> norm_dist;
    std::vector<double> v_i;
    std::vector<double> v_x;
    double T(unsigned i) const {
        return this->T0*exp(-double(i)/this->tau);
    }
    double metropolis(double x0, double x1, unsigned i) {
        double delta_E = this->E(x1) - this->E(x0);
        if (P(delta_E, T(i)) < this->norm_dist(this->generator) ) {
            return x0;
        } else {
            return x1;
        }
    }
public:
    Impl(
        std::function<double(double)> E, 
        double T0, 
        double Tf,
        double tau,
        double x,
        double scale
    ) : 
        generator(std::time(nullptr)),
        E(E), 
        T0(T0),
        Tf(Tf),
        tau(tau),
        x(x),
        scale(scale),
        norm_dist(std::normal_distribution<double>(0.0, scale))
    {
        assert(T0 > Tf && T0 > 0 && Tf > 0);
    }
    double operator()() {
        double delta_x;
        double x_new;
        for (int i = 0; T(i) > this->Tf; i++) {
            delta_x = this->norm_dist(this->generator)*T(i);
            x_new = this->x + delta_x;
            this->x = this->metropolis(this->x, x_new, i);
            this->v_i.push_back(i);
            this->v_x.push_back(this->x);
        }
        return this->x;
    }
    void show_plot() {
        auto f = matplot::figure(false);
        auto ax = matplot::gca();
        auto p = ax->plot(this->v_i, this->v_x);
        f->show();
    }
};

Anneal::Anneal(
    std::function<double(double)> E, 
    double T0,
    double Tf,
    double tau,
    double x,
    double scale
) :
    pimpl(new Impl(E, T0, Tf, tau, x, scale)) 
{}
double Anneal::operator()() {
    return (*this->pimpl)();
}
void Anneal::show_plot() {
    this->pimpl->show_plot();
}
Anneal::~Anneal() {
    delete this->pimpl;
}
