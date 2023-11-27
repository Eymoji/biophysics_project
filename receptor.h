#ifndef RECEPTOR_H
#define RECEPTOR_H

#include<ostream>
#include<cmath>
#include<vector>
#include"fonctions.h"
#include"chemo.h"

using namespace std;

class chemo;

class receptor {

public:

    //position//
    double x{};
    double y{};

    //radius of chemo//
    double size{};

    //number of molecules absorbed by the the receptor//
    int n{};

    //Average time a molecule remains bonded to a receptor when absorbed
    double Tau_B{};

    receptor();
    receptor(double s, double a, double theta, double Lx, double Ly, double Tau);
    ~receptor();

    //Absorption of a chemo by a receptor
    void absorption(vector<chemo>& vector_chemo);
    void release(double Lx0, double Ly0, double a, double D, double D_prime, double dt, vector<chemo>& vector_chemo);
    
};


#endif //BIOPHYSICS_PROJECT_CHEMO_H