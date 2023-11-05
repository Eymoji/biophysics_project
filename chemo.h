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
    double x;
    double y;
    
    //velocity//
    double vx;
    double vy;

    //radius of chemo//
    double r;

    //diffusion properties//
    double D1;
    double D2;

    chemo();
    chemo(double Taillesurface, double a, double D1, double D2);
    ~chemo();

    //Chemo's behaviour
    void diffusion_langevin(double eta, double dt);
    
    //Absoprtion of a molecule by a receptor
    void absorbed(vector<receptor> detection_system, double s, double a, double Taillesurface);

    //Boundary conditions
    void boundary_conditions(double Taillesurface);

    //Update of position
    bool in_the_cell(double a, double Taillesurface);
    void update_position(double dt);
};


#endif //BIOPHYSICS_PROJECT_CHEMO_H