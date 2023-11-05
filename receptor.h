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
    double x;
    double y;

    //radius of chemo//
    double size;

    //number of molecules absorbed by the the receptor//
    int n;

    receptor();
    receptor(double s, double a, double theta);
    ~receptor();

    //Absorption of a chemo by a receptor
    void absorption(vector<chemo> groupe);
    
};


#endif //BIOPHYSICS_PROJECT_CHEMO_H