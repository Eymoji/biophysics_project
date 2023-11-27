#include<iostream>
#include<ostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<vector>
#include <chrono>
#include <cstdlib>
#include<execution>
#include"receptor.h"
#include"chemo.h"
//#include <tbb/parallel_for_each.h>

using namespace std;

int main() {

    /** PARAMETERS **/

    //Hyperparameters
    const double dt = 0.1;                    //Time interval between two iterations
    const double time_max = 10;               //Simulation time
    const double Lx = 600;                    //Simulation length on each axis
    const double Ly = 600;

    //Properties of the cell
    const double Rcell = 100;                 //cell radius
    const double VXcell = 1.0;                //cell speed
    const double VYcell = 1.0;

    //Properties of chemoattractants
    const double D1 = 100;                     //Diffusion coefficient in the volume
    const double D2 = 1;                      //Diffusion coefficient on the surface of the cell
    const double cinf = 0.05;                  //Concentration at long distance
    const int Nchemo = int(cinf * Lx * Ly);   //Number of chemoattractants in the system

    //Properties of receptorss
    const double Rrec = 5;                    //Receptors radius
    const int Nrec = 25;                      //Number of receptors

    //Properties of the ambient medium
    const double eta = 0.1;                   //Viscosity of the medium

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
        receptor_vector[i] = receptor (Rrec, Rcell, 2. * M_PI * i / Nrec, Lx, Ly);
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

            auto rt = chrono::high_resolution_clock::now();
            auto rt_past = chrono::duration_cast<chrono::microseconds>(rt - rt0);
            auto rt_future = 1.e-6 * rt_past.count() * (1 - t * dt / time_max) / (t * dt / time_max);

            cout << "\tTemps restant estime : " << int(rt_future) << " secondes          " << "\r";
        }

        std::for_each(
                std::execution::par,
                chemo_vector.begin(),
                chemo_vector.end(),
                [&dt, &VXcell, &eta, &VYcell](auto&& chem){
                    chem.diffusion_langevin(eta, dt);
                    chem.update_position(dt, VXcell*dt, VYcell*dt);}
                );

        std::for_each(
                std::execution::par,
                receptor_vector.begin(),
                receptor_vector.end(),
                [&chemo_vector](auto&& rec){
                    rec.absorption(chemo_vector);}
        );


        //Each molecule: We keep it on the map, outside the cell
        for(int i = 0; i < Nchemo; i++)
            chemo_vector[i].boundary_conditions();

        for(int i = 0; i < Nrec; i++){
            nbr_absorption << receptor_vector[i].n << " ";
        }

        for (int i = 0; i < Nchemo; i++) {
            coordinate_x << int(chemo_vector[i].x) << " ";
            coordinate_y << int(chemo_vector[i].y) << " ";
        }
        nbr_absorption << endl;
        coordinate_x << endl;
        coordinate_y << endl;
    }

    //We close our files to save data
    coordinate_x.close();
    coordinate_y.close();
    nbr_absorption.close();
    coord_receptor.close();

    return 0;
}