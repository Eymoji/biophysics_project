#include<iostream>
#include<ostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<vector>
#include <chrono>
#include"receptor.h"
#include"chemo.h"
#include <cstdlib>

using namespace std;

int main() {

    /** PARAMETERS **/

    //Hyperparameters
    const double dt = 0.1;                      //Time interval between two iterations
    const double time_max = 2000;                 //Simulation time
    const double Lx = 600;                      //Simulation length on each axis
    const double Ly = 600;

    //Properties of the cell
    const double Rcell = 100;                   //cell radius
    const double VXcell = 0;                  //cell speed
    const double VYcell = 0;

    //Properties of chemoattractants
    const double D1 = 1;                      //Diffusion coefficient in the volume
    const double D2 = 1;                        //Diffusion coefficient on the surface of the cell
    const double cinf = 0.0001;                    //Concentration at long distance
    const int Nchemo = int(cinf * Lx * Ly);     //Number of chemoattractants in the system

    //Properties of receptorss
    const double Rrec = 5;                      //Receptors radius
    const int Nrec = 25;                        //Number of receptors
    const double Tau_B = 3.;                    //Average time a receptor keeps a captured molecule

    //Properties of the ambient medium
    const double eta = 0.01;                     //Viscosity of the medium

    //Save parameters
    ofstream parameters("data/param.csv");
    parameters << "Rcell,Rrec,Nrec,time_max,dt,Lx,Ly" << endl;
    parameters << Rcell << "," << Rrec << "," << Nrec << "," << time_max << "," << dt << "," << Lx << "," << Ly << endl;
    parameters.close();


    /** INITIALISATION **/

    //Generation of the random map parameters
    vector<double> W = random_landscape_initializer();

    //Creation of a vector of chemoattractants
    vector<chemo> chemo_vector (Nchemo);
    for(int i = 0; i < Nchemo; i++)
        chemo_vector[i] = chemo (Lx, Ly, Rcell, D1, D2, W);

    //Creation of a vector of receptors (they are only points with two coordinates and size Rrec)
    vector<receptor> receptor_vector (Nrec);
    ofstream coord_receptor ("data/rec.txt");
    for(int i = 0; i < Nrec; i++) {
        receptor_vector[i] = receptor (Rrec, Rcell, 2. * M_PI * i / Nrec, Lx, Ly, Tau_B);
        coord_receptor << receptor_vector[i].x << " " << receptor_vector[i].y << " " << endl;
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
        for(unsigned int i = 0; i < chemo_vector.size(); i++) {
            chemo_vector[i].diffusion_langevin(eta, dt);
            chemo_vector[i].update_position(dt, VXcell*dt, VYcell*dt);
        }

        //Each receptor absorbs a molecule, if so, it can release it in the medium with a rate 1/Tau_B
        for(int i = 0; i < Nrec; i++){
            receptor_vector[i].release(Lx, Ly, Rcell, D1, D2, dt, chemo_vector);
            receptor_vector[i].absorption(chemo_vector);
            nbr_absorption << receptor_vector[i].n << " ";
        }
        nbr_absorption << endl;

        //Each molecule: We keep it on the map, outside the cell
        for(unsigned int i = 0; i < chemo_vector.size(); i++)
            chemo_vector[i].boundary_conditions();


        for (unsigned int i = 0; i < chemo_vector.size(); i++) {
            coordinate_x << chemo_vector[i].x << " ";
            coordinate_y << chemo_vector[i].y << " ";
        }
        for (int i = chemo_vector.size(); i < Nchemo; i++) {
            coordinate_x << "NaN" << " ";
            coordinate_y << "NaN" << " ";
        }
        coordinate_x << endl;
        coordinate_y << endl;
    }

    //We close our files to save data
    coordinate_x.close();
    coordinate_y.close();
    nbr_absorption.close();
    coord_receptor.close();

    //system("python display.py");
    return 0;
}
