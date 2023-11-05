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


receptor::~receptor() {
}


void receptor::absorption(vector<chemo> groupe) {
    class chemo;
    for (unsigned int i; i < groupe.size(); i++)
        if (norm(groupe[i].x - x, groupe[i].y - y) < size) 
            n += 1;    
}