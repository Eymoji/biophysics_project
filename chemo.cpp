#include "chemo.h"
using namespace std;

chemo::chemo()= default;

chemo::chemo(double Lx0, double Ly0, double a, double D, double D_prime) {
    //We randomly initialize the position of the chemo on the surface, outside of the bacterium, and set their initial velocity to 0
    Lx = Lx0;
    Ly = Ly0;
    Rcell = a;

    reset_chemo();

    D1 = D;
    D2 = D_prime;
}

void chemo::reset_chemo(){
    do {
        x = titled_distribution(0, Lx, 0.8, 0.5, 1.);
        y = titled_distribution(0, Ly, 0.8, 0.5, 1.);
    } while (in_the_cell(Rcell) or x<0 or x>Lx or y<0 or y>Ly);

    vx = 0;
    vy = 0;
};

chemo::~chemo() = default;


void chemo::diffusion_langevin(double eta, double dt) {

    //We first apply a random force on the molecule
    
    //We randomly choose the intensity of the force such that <F²> = 2D / dt -> Fmax = sqrt(6D)
    double Fmax = sqrt(6*D1);
    double F = uniform_distribution(-Fmax,Fmax);

    //We now choose the orientation of the force that will apply on the chemo
    double theta = uniform_distribution(0,2*M_PI);

    //We modify the velocity of the molecule
    double r=1;
    vx = vx + dt * (F*cos(theta) - 6*M_PI*eta*r*vx);
    vy = vy + dt * (F*sin(theta) - 6*M_PI*eta*r*vy);
}


bool chemo::in_the_cell(double a) const { //Tests if the molecule is in the cell
    return (norm2(x - Lx/2, y - Ly/2) < a*a);
}


void chemo::update_position(double dt) {
    x = x + vx*dt;
    y = y + vy*dt;
}


void chemo::boundary_conditions() {
    //We keep all the molecules onr our surface, which is consistent since the medium around the bacterium
    //is considered to be infinite, and we want to keep the same number of molecules in the system.
    if (x < 0)  x += Lx;
    if (x > Lx)  x -= Lx;
    if (y < 0)  y += Ly;
    if (y > Ly)  y -= Ly;

    if (norm2(x-Lx/2,y-Ly/2) < Rcell*Rcell){
        double xb = Rcell*cos(theta(x-Lx/2,y-Ly/2));
        double yb = Rcell*sin(theta(x-Lx/2,y-Ly/2));
        x = Lx/2 + xb + (xb - (x-Lx/2));
        y = Ly/2 + yb + (yb - (y-Ly/2));

    }
}
