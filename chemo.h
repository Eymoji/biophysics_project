#pragma once

#include <cmath>
#include <functional>
#include"fonctions.h"
#include<iostream>

using namespace std;
class receptor;

class chemo {

public:

    chemo();

    double x, y;            //position//
    double vx, vy;          //velocity//
    double D1, D2;          //diffusion properties//
    double Lx, Ly, Rcell;   //map properties//
    bool absorbed;          //Tells us if the chemo is absorbed or not

    // Constructor, reset and destructor
    chemo(double Lx0, double Ly0, double a, double D, double D_prime, const std::vector<double>& W);
    
    void reset_chemo();
    ~chemo();

    //Chemo's behaviour
    void diffusion_langevin(double eta, double dt);

    //Boundary conditions
    void boundary_conditions();

    //Update of position
    bool in_the_cell(double a) const;
    void update_position(double dt, double VxCell, double VyCell);
    void released(double x_rec, double y_rec, double r_rec, double dt);
};