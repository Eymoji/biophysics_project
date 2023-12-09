#include "chemo.h"
using namespace std;

chemo::chemo()= default;

chemo::chemo(double Lx0, double Ly0, double a, double D, double D_prime, const vector<double>& W) {
    //We randomly initialize the position of the chemo on the surface, outside of the bacterium, and set their initial velocity to 0
    Lx = Lx0;
    Ly = Ly0;
    Rcell = a;

    do {
        vector<double> w = random_landscape_distribution(Lx, Ly, W);
        x = w[0];
        y = w[1];
//        x = titled_distribution(0, Lx, 0.0, 0.2, 1.);
//        y = titled_distribution(0, Ly, 0.0, 0.2, 1.);
    } while (in_the_cell(Rcell));

    vx = 0;
    vy = 0;

    D1 = D;
    D2 = D_prime;

    absorbed = false;
}


void chemo::reset_chemo(){
    do {
        x = uniform_distribution(0, Lx);
        y = uniform_distribution(0, Ly);
    } while (in_the_cell(Rcell));

    vx = 0;
    vy = 0;
};

chemo::~chemo() = default;


void chemo::diffusion_langevin(double eta, double dt) {

    //Random force on the molecule, intensity such that <F²> = 2D / dt -> Fmax = sqrt(6D)
//    double Fmax = sqrt(6*D1);
    double Fmax = sqrt(6*D1/dt);
    double F = uniform_distribution(0,Fmax);

    //We now choose the orientation of the force that will apply on the chemo
    double theta = uniform_distribution(0,2*M_PI);

    //We modify the velocity of the molecule
    double r=1;
//    vx = vx + dt * (F*cos(theta) - 6*M_PI*eta*r*vx);
//    vy = vy + dt * (F*sin(theta) - 6*M_PI*eta*r*vy);
    vx = dt * F*cos(theta);
    vy = dt * F*sin(theta);
}


bool chemo::in_the_cell(double a) const { //Tests if the molecule is in the cell
    return (norm2(x - Lx/2, y - Ly/2) < a*a);
}


void chemo::update_position(double dt, double VxCell, double VyCell) {
    x = x + vx*dt;
    y = y + vy*dt;

    // relative cell movement : stokes regime
    double r = norm(x-Lx/2,y-Ly/2);
    double th = theta(x-Lx/2,y-Ly/2);
    double thV = theta(VxCell,VyCell);
    double v0 = norm(VxCell,VyCell);
    double vr = - v0 * cos(th) * (1 - (3*Rcell)/(2*r) + (Rcell*Rcell*Rcell)/(2*r*r*r));
    double vth = v0 * sin(th) * (1 - (3*Rcell)/(4*r) - (Rcell*Rcell*Rcell)/(4*r*r*r));
    x = x + (vr*cos(th-thV) - vth*sin(th-thV))*dt;
    y = y + (vr*sin(th-thV) + vth*cos(th-thV))*dt;
}


void chemo::boundary_conditions() {
    //We keep all the molecules onr our surface, which is consistent since the medium around the bacterium
    //is considered to be infinite, and we want to keep the same number of molecules in the system.
    if (x < 0)  x += Lx;
    if (x > Lx)  x -= Lx;
    if (y < 0)  y += Ly;
    if (y > Ly)  y -= Ly;

    if (norm2(x-Lx/2.,y-Ly/2.) < Rcell*Rcell){
        double xb = Rcell*cos(theta(x-Lx/2,y-Ly/2));
        double yb = Rcell*sin(theta(x-Lx/2,y-Ly/2));
        x = Lx/2 + xb + (xb - (x-Lx/2));
        y = Ly/2 + yb + (yb - (y-Ly/2));

    }
}

//Initializer of a molecule released by a receptor
void chemo::released(double x_rec, double y_rec, double r_rec, double dt) {

    double theta_rec = theta(x_rec, y_rec);
    double theta_release = uniform_distribution(M_PI/2., M_PI/2.) + theta_rec;

    x = x_rec + 1.1*r_rec*cos(theta_release);
    y = y_rec + 1.1*r_rec*sin(theta_release);
	
    vx = D1*dt*cos(theta_release);
    vy = D1*dt*sin(theta_release);

    absorbed = false;
}