#include<iostream>
#include<ostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<random>
#include<vector>
#include <chrono>
#include"receptor.h"
#include"chemo.h"
#include"fonctions.h"

using namespace std;

int main() {

    /** PARAMETERS **/

    //Properties of the cell
    double Rcell = 100;                 //cell radius

    //Properties of chemoattractants
    int Nchemo;                         //Number of chemoattractants in the system
    double D1 = 10;                      //Diffusion coefficient in the volume
    double D2 = 1;                      //Diffusion coefficient on the surface of the cell
    double cinf = 0.05;                  //Concentration at long distance

    //Properties of receptors
    double Rrec = 3;                    //Receptors radius
    int Nrec = 60;                     //Number of receptors

    //Properties of the ambient medium
    double eta = 0.2;                     //Viscosity of the medium

    //Hyperparameters
    double dt = 0.1;                    //Time interval between two iterations
    double time_max = 200;               //Simulation time
    double L = 400;                    //Simulation length on each axis

    //Save parameters
    ofstream parameters("data/param.csv");
    parameters << "Rcell,Rrec,Nrec,time_max,dt,L" << endl;
    parameters << Rcell << "," << Rrec << "," << Nrec << "," << time_max << "," << dt << "," << L << endl;
    parameters.close();


    /** INITIALISATION **/

    //Creation of a vector of chemoattractants
    Nchemo = int(cinf * L * L);
    vector<chemo> chemo_vector (Nchemo);
    for(int i = 0; i < Nchemo; i++)
        chemo_vector[i] = chemo (L, Rcell, D1, D2);


    //Creation of a vector of receptors (they are only points with two coordinates and size Rrec)
    vector<receptor> receptor_vector (Nrec);
    ofstream coord_receptor ("data/rec.txt");
    for(int i = 0; i < Nrec; i++) {
        receptor_vector[i] = receptor (Rrec, Rcell, 2. * M_PI * i / Nrec, L);
        coord_receptor << receptor_vector[i].x << " " << receptor_vector[i].y << " " << std::endl;
    }

    //Save the position of particles, one for x coordinates, one for y coordinates, and information given by receptors
    ofstream coordinate_x("data/x.txt");
    ofstream coordinate_y ("data/y.txt");
    ofstream nbr_absorption ("data/nbr_absorption.txt");

    //We write the coordinates of molecules during the first iteration
    for (int i = 0; i < Nrec; i++) {
        nbr_absorption << receptor_vector[i].n << " ";
    }
    nbr_absorption << endl;


    /** EVOLUTION TIME **/
    auto rt0 = std::chrono::high_resolution_clock::now();
    for (int t=0; t<=time_max/dt; t++) {
        if(int(1000 * t * dt / time_max) % 10 == 0) {
            cout << "Progression : " << int(100 * t * dt / time_max) << " %";

            auto rt = std::chrono::high_resolution_clock::now();
            auto rt_past = std::chrono::duration_cast<std::chrono::microseconds>(rt - rt0);
            auto rt_future = 1.e-6 * rt_past.count() * (1 - t * dt / time_max) / (t * dt / time_max);
            cout << "\tTemps restant estime : " << int(rt_future) << " secondes          " << "\r";
        }

        //Each molecule: We compute the new velocity and update the position of the molecule
        for(int i = 0; i < Nchemo; i++) {
            chemo_vector[i].diffusion_langevin(eta, dt);
            chemo_vector[i].update_position(dt);
        }

        //Each receptor absorbs a molecule, if so, it is activated and the molecule is randomly replaced
        for(int i = 0; i < Nrec; i++){
            receptor_vector[i].absorption(chemo_vector, L);
            nbr_absorption << receptor_vector[i].n << " ";
        }
        nbr_absorption << endl;

        //Each molecule: We keep it on the map, outside the cell
        for(int i = 0; i < Nchemo; i++)
            chemo_vector[i].boundary_conditions(L,Rcell);


        for (int i = 0; i < Nchemo; i++) {
            coordinate_x << chemo_vector[i].x << " ";
            coordinate_y << chemo_vector[i].y << " ";
        }
        coordinate_x << endl;
        coordinate_y << endl;
    }

    //We close our files to save data
    coordinate_x.close();
    coordinate_y.close();
    nbr_absorption.close();
    parameters.close();
    coord_receptor.close();

    return 0;
}