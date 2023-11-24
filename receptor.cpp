#include "receptor.h"
using namespace std;

receptor::receptor() = default;

receptor::receptor(double s, double a, double theta, double Lx, double Ly) {
    //We distribute the receptors on the surface of the bacterium
    x = a*cos(theta) + Lx/2;
    y = a*sin(theta) + Ly/2;
    size = s;
    n=0;
}


receptor::~receptor() = default;


void receptor::absorption(vector<chemo>& vector_chemo) {
    n = 0;
    for (auto & c : vector_chemo)
        if (norm2(c.x - x, c.y - y) < size*size) {
            n += 1;
            c.reset_chemo();
        }
}
