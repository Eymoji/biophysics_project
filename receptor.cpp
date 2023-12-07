#include "receptor.h"
using namespace std;

receptor::receptor() = default;

receptor::receptor(double s, double a, double theta, double Lx, double Ly, double Tau) {
    //We distribute the receptors on the surface of the bacterium
    x = a*cos(theta) + Lx/2.;
    y = a*sin(theta) + Ly/2.;
    size = s;
    n=0;
    Tau_B = Tau;
}


receptor::~receptor() = default;


void receptor::absorption(vector<chemo>& vector_chemo) {
    auto chemo = vector_chemo.begin();

    while (chemo != vector_chemo.end()) {
        auto &chem = *chemo; // Référence à l'élément actuel

        if (norm2(chem.x - x, chem.y - y) < size * size) {
            n += 1; // Incrémentation du compteur d'activations du récepteur
            // Effacer l'élément du vecteur
            chemo = vector_chemo.erase(chemo);
        }

        else {
            // Passer à l'élément suivant
            ++chemo;
        }
    }
}

void receptor::release(double Lx0, double Ly0, double a, double D, double D_prime, double dt, vector<chemo>& vector_chemo) {
    double p;   //random variable that determines if a molecule is released by the receptor
    if (n > 0) {  //We check if the receptor is not empty

        for (int i=0; i<n; i++) {   //For all the molecules captured by the receptor
            p = uniform_distribution(0, Tau_B);

            if (p <= dt) {  //The molecule is released by the recptor if this is realized
                n -= 1;
                chemo C(Lx0, Ly0, a, D, D_prime, x, y, size, dt);
                vector_chemo.push_back(C);
                
            }
        }  
    }
}
