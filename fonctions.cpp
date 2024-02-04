#include<iostream>
#include<cmath>
#include<random>
#include"fonctions.h"
#include<vector>

using namespace std;

//Initialisation de la graine des processus aléatoires
random_device rdf;
default_random_engine engf(rdf());
uniform_real_distribution<float> distf(0., 1.);


double rejetgaussien(double moyenne, double variance) { // Fournit valeur alea. de densite gaussienne
    double x, y, ecarttype = sqrt(variance); // avec variance var et moyenne 0.
    do { // On prend x dans l'intervalle [-5sigma,5sigma], on suppose que la
        x = 10.*ecarttype * distf(engf) - 5.*ecarttype + moyenne; // gaussienne est nulle ailleurs.
        y = distf(engf);
    } while(y > exp(-(x-moyenne)*(x-moyenne)/(2.*variance)));
    return x;
}

double uniform_distribution(double min, double max) {
  double x;
  x = (max-min) * distf(engf) + min;
  return x;
}

double norm(double x, double y) {
  double n = sqrt(x*x+y*y);
  return n;
}

double norm2(double x, double y) {
    return x*x+y*y;
}

double periodic_dx(double x1, double x2, double Lx){
    return min(abs(x2-x1), abs(abs(x2-x1)-Lx));
}

double theta(double x, double y){
    return atan2(y,x);
}

double titled_distribution(double Xmin, double Xmax, double x0, double ratio, double lambda){
    double x, c;

    do {
        x = distf(engf); // uniform distribution [0,1]
        c = distf(engf);
    } while (c > ratio + (1-ratio) * cos((x-x0)*M_PI*lambda)*cos((x-x0)*M_PI*lambda));

    x = (Xmax-Xmin) * x + Xmin;
    return x;
}

int Nw = 10;

vector<double> random_landscape_initializer(){
    vector<double> W (4*Nw);
    for (int i = 0; i < 4*Nw; ++i) {
        W[i] = distf(engf);
    }
    return W;
}

vector<double> random_landscape_distribution(double Lx, double Ly, const vector<double>& W){
    double x, y, c, cmin, cmax, f;

    do {
        x = distf(engf); // uniform distribution [0,1]
        y = distf(engf);
        c = distf(engf);
        cmin = 0;
        cmax = 0;

        f = 0;

        for (int i = 0; i < Nw; ++i) {
            double dx = periodic_dx(x,W[i],1);
            double dy = periodic_dx(y,W[Nw+i],1);
            double r0 = W[2*Nw+i]/5;
            double a0 = W[3*Nw+i];

            f += a0*exp(-(dx*dx+dy*dy)/(2*r0*r0));

            if(a0 > 0)
                cmax += a0;
            else
                cmin += a0;
        }


    } while (((c * (cmax - cmin) + cmin) > f));

    vector<double> w (2);
    w[0] = Lx * x;
    w[1] = Ly * y;
    return w;
}



