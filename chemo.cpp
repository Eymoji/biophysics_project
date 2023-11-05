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


chemo::~chemo() {
}


void chemo::diffusion_langevin(double eta, double dt) {

    //We first apply a random force on the molecule
    
    //We randomly choose the intensity of the force such that <F²> = 2D / dt -> Fmax = sqrt(6D)
    double Fmax = sqrt(6*D1);
    double F = uniform_distribution(-Fmax,Fmax);

    //We now choose the orientation of the force that will apply on the chemo
    double theta = uniform_distribution(0,2*M_PI);

    //We modify the velocity of the molecule
    vx = vx + dt * (F*cos(theta) - 6*M_PI*eta*r*vx);
    vy = vy + dt * (F*sin(theta) - 6*M_PI*eta*r*vy);
}


bool chemo::in_the_cell(double a, double Taillesurface) {
    //The molecules follow a Brownian Motion but can't enter the cell without going through a receptor
    //This function tests if the molecule is out of the cell in the next iteration
    if (norm(x + vx - Taillesurface/2, y + vy - Taillesurface/2) < a)
        return true;
    else
        return false;
}


void chemo::update_position(double dt) {
    x = x + vx*dt;
    y = y + vy*dt;
}


void chemo::absorbed(vector<receptor> detection_system, double s, double a, double Taillesurface) {
    //If a molecule is absorbed, we send it back at long distance from the bacterium to keep a constant density of molecule
    //during the  simulation. To do so, we determine a the ratio between the size of the surface and th size of the bacterium
    double ratio = Taillesurface/a;

    for (unsigned int i; i < detection_system.size(); i++) {
        if (norm(detection_system[i].x - x, detection_system[i].y - y) < s) {
            x = Taillesurface/2 + ratio/2 * (x - Taillesurface/2);
            y = Taillesurface/2 + ratio/2 * (y - Taillesurface/2);
        }
    }
}


void chemo::boundary_conditions(double Taillesurface) {
    //We keep all the molecules onr our surface, which is consistent since the medium around the bacterium
    //is considered to be infinite, and we want to keep the same number of molecules in the system.
    if (x < 0)
        x += Taillesurface;
    if (x > Taillesurface)
        x -= Taillesurface;
    if (y < 0)
        y += Taillesurface;
    if (y > Taillesurface)
        y -= Taillesurface;
}