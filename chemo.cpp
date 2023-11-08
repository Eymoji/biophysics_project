#include "chemo.h"
using namespace std;

chemo::chemo() {
    x = 0;
    y = 0;
    vx = 0;
    vy = 0;
    D1 = 0;
    D2 = 0;
}


chemo::chemo(double Taillesurface, double a, double D, double D_prime) {
    //We randomly initialize the position of the chemo on the surface, outside of the bacterium, and set their initial velocity to 0
    do {
        x = uniform_distribution(0, Taillesurface);
        y = uniform_distribution(0, Taillesurface);
    } while (norm(x - Taillesurface/2, y - Taillesurface/2) < a);

    vx = 0;
    vy = 0;

    D1 = D;
    D2 = D_prime;
}


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


bool chemo::in_the_cell(double a, double Taillesurface) const {
    //This function tests if the molecule is out of the cell in the next iteration
    return (norm(x + vx - Taillesurface/2, y + vy - Taillesurface/2) < a);
}


void chemo::update_position(double dt) {
    x = x + vx*dt;
    y = y + vy*dt;
}


void chemo::boundary_conditions(double L) {
    //We keep all the molecules onr our surface, which is consistent since the medium around the bacterium
    //is considered to be infinite, and we want to keep the same number of molecules in the system.
    if (x < 0)  x += L;
    if (x > L)  x -= L;
    if (y < 0)  y += L;
    if (y > L)  y -= L;
}
