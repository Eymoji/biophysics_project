#include<iostream>
#include<ostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<random>
#include<vector>
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
    double D1 = 3;                      //Diffusion coefficient in the volume
    double D2 = 1;                      //Diffusion coefficient on the surface of the cell
    double cinf = 0.002;                //Concentration at long distance

    //Properties of receptors
    double Rrec = 1;                    //Receptors radius
    int Nrec = 100;                     //Number of receptors

    //Properties of the ambient medium
    double eta = 0;                     //Viscosity of the medium

    //Hyperparameters
    double dt = 0.1;                    //Time interval between two iterations
    double time_max = 50;               //Simulation time
    double L = 2000;                    //Simulation length on each axis

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
    for(int i = 0; i < Nrec; i++)
        receptor_vector [i] = receptor (Rrec, Rcell, 2. * M_PI * i / Nrec);


    //Save the position of particles, one for x coordinates, one for y coordinates, and information given by receptors
    ofstream coordinate_x("data/x.txt");
    ofstream coordinate_y ("data/y.txt");
    ofstream nbr_absorption ("data/nbr_absorption.txt");
    nbr_absorption << "time/receptor ";
    for (int i = 0; i < Nchemo; i++)
        nbr_absorption << to_string(i) << " ";    //The first line of file gives us the name of receptor//
    nbr_absorption << endl;

    //We write the coordinates of molecules during the first iteration
    nbr_absorption << 0 << " ";                   //The first column of file gives us the time//
    for (int i = 0; i < Nrec; i++) {
        nbr_absorption << receptor_vector[i].n << " ";
    }
    nbr_absorption << endl;


    /** EVOLUTION TIME **/

    for (int t=0; t<=time_max/dt; t++) {
        cout << "Progression : " << 100 * t * dt / time_max << " %" << "\r";

        //For each receptor:
        for(int i = 0; i < Nrec; i++)
            // Test if it absorbs a molecule, if so, is activated and the molecule ii randomly replaced
            receptor_vector[i].absorption(chemo_vector, L);

        //For each molecule:
        for(int i = 0; i < Nchemo; i++) {

            //We compute the new velocity while the molecule doesn't enter the bacterium
            do {
                chemo_vector[i].diffusion_langevin(eta, dt);
            } while (chemo_vector[i].in_the_cell(Rcell, L));

            //We update the position of the molecule
            chemo_vector[i].update_position(dt);

            //We keep it on the surface
            chemo_vector[i].boundary_conditions(L);
        }


        for (int i = 0; i < Nchemo; i++) {
            coordinate_x << chemo_vector[i].x << " ";
            coordinate_y << chemo_vector[i].y << " ";
        }
        coordinate_x << endl;
        coordinate_y << endl;

        //We write the number of molecules absorbed by each receptor until this iteration
        for (int i = 0; i < Nrec; i++)
            nbr_absorption << receptor_vector[i].n << " ";
        nbr_absorption << endl;

    }

    //We close our files to save data
    coordinate_x.close();
    coordinate_y.close();
    nbr_absorption.close();
    parameters.close();

    return 0;
}