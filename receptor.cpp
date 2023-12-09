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

    //A receptor can absorb a molecule only if it is empty
    if (n==0) {

        //If a few chemo come into the receptor during one iteration, only the one which is the nearest from the center of the receptor will
        //be absorbed

        //We create dmin to determine which chemo is the nearest from the receptor
        double dmin = 100;
        //We define the index of the chemo that will be absorbed, equal to -1 if there is no cheo absorbed
        int i_absorbed = -1;

        //We create a vector of indices containing the index of each chemo that enters the receptor without beiing absorbed
        vector<int> not_absorbed;

        int L = vector_chemo.size();
        for (int i = 0; i < L; i++) {
            //We check if the chemo enter the receptor
            if (norm2(vector_chemo[i].x - x, vector_chemo[i].y - y) < size * size) {
                //The receptor is now occupied
                n = 1;

                //We check if the chemo is the nearest from the center of the receptor or not
                if (norm2(vector_chemo[i].x - x, vector_chemo[i].y - y) < dmin * dmin) {
                    //If there is one, the previous chemo that was absorbed is no longer absorbed
                    if (i_absorbed > -1) {
                        not_absorbed.push_back(i_absorbed);
                    }

                    //We change the chemo that will be absorbed
                    dmin = norm(vector_chemo[i].x - x, vector_chemo[i].y - y);
                    i_absorbed = i;
                }
                
                //If a chemo enters the receptor but is not absorbed
                else
                    not_absorbed.push_back(i);
            }
        }
        //We do nothing for chemo that entered the redceptor without being absorbed, but we can if we want
        //for (int i : not_absorbed) {
            //do {

            //} while (norm(vector_chemo[i].x - x, vector_chemo[i].y - y) < size * size)
        //}

        //we make disappear the chemo that has been absorbed, if there is one
        if (i_absorbed > -1)
            vector_chemo.erase(vector_chemo.begin() + i_absorbed);
    }
}


void receptor::release(double Lx0, double Ly0, double a, double D, double D_prime, double dt, vector<chemo>& vector_chemo) {
    double p;   //random variable that determines if a molecule is released by the receptor
    if (n == 1) {  //We check if the receptor is not empty

        p = uniform_distribution(0, Tau_B);

        if (p <= dt) {  //The molecule is released by the recptor if this is realized
            n = 0;
            chemo C(Lx0, Ly0, a, D, D_prime, x, y, size, dt);
            vector_chemo.push_back(C);
        }  
    }
}