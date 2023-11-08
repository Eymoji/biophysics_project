#include "receptor.h"
using namespace std;

receptor::receptor() {
    x = 0;
    y = 0;
    size = 0;
    n = 0;
}


receptor::receptor(double s, double a, double theta) {
    //We distribute the receptors on the surface of the bacterium
    x = a*cos(theta);
    y = a*sin(theta);
    size = s;
    n=0;
}


receptor::~receptor() = default;


void receptor::absorption(vector<chemo>& vector_chemo, double L) {
    n = 0;
    for (auto & chemo : vector_chemo)
        if (norm(chemo.x - x, chemo.y - y) < size) {
            n += 1;
            chemo.x = uniform_distribution(0, L);
            chemo.y = uniform_distribution(0, L);
        }
}
