#ifndef CHEMO_H
#define CHEMO_H

#include<ostream>
#include<cmath>
#include<vector>
#include"receptor.h"
#include"fonctions.h"

using namespace std;
class receptor;

class chemo {

public:

    //position//
    double x, y;
    
    //velocity//
    double vx, vy;

    //diffusion properties//
    double D1, D2;

    chemo();
    chemo(double Taillesurface, double a, double D1, double D2);
    ~chemo();

    //Chemo's behaviour
    void diffusion_langevin(double eta, double dt);
    
    //Absoprtion of a molecule by a receptor
    void absorbed(const vector<receptor>& detection_system, double s, double a, double L);

    //Boundary conditions
    void boundary_conditions(double L, double Rcell);

    //Update of position
    bool in_the_cell(double a, double Taillesurface) const;
    void update_position(double dt);
};


#endif //BIOPHYSICS_PROJECT_CHEMO_H