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
    double D1 = 1;                      //Diffusion coefficient in the volume
    double D2 = 1;                      //Diffusion coefficient on the surface of the cell
    double cinf = 0.001;                //Concentration at long distance

    //Properties of receptors
    double Rrec = 1;                    //Receptors radius
    int Nrec = 100;                     //Number of receptors

    //Properties of the ambient medium
    double eta = 0;                     //Viscosity of the medium

    //Hyperparameters
    double dt = 0.1;                    //Time interval between two iterations
    double time_max = 50;               //Simulation time
    double L = 2000;                    //Simulation length on each axis

    fstream parameters;
    parameters.open("data/param.csv",ios::out);
    parameters << "Rcell,Rrec,Nrec,time_max,dt,L" << endl;
    parameters << Rcell << "," << Rrec << "," << Nrec << "," << time_max << "," << dt << "," << L << endl;
    parameters.close();


    /** INITIALISATION **/

    //Creation of a vector of chemo
    vector<chemo> chemo_vector;
    Nchemo = int(cinf * L * L);
    cout << "Number of chemoattractants : " << Nchemo << std::endl;
    for(int i = 0; i < Nchemo; i++) {
        chemo C(L, Rcell, D1, D2);
        chemo_vector.push_back(C);
    }

    //Creation of a vector of receptors (they are only points with two coordinates and size Rrec)
    vector<receptor> receptor_vector;
    cout << "Number of receptors : " << Nrec << std::endl;
    for(int i = 0; i < Nrec; i++) {
        receptor R(Rrec, Rcell, 2. * M_PI * i / Nrec);     //Receptors are first distributed around the bacterium//
        receptor_vector.push_back(R);
    }



    //We want to write our data in a file to exploit it in python

    //We save the position of particles, one for x coordinates, one for y coordinates
    fstream coordinate_x;
    fstream coordinate_y;

    coordinate_x.open("data/x.txt", ios::out);
    coordinate_x << "time/molecule ";
    coordinate_y.open("data/y.txt", ios::out);
    coordinate_y << "time/molecule ";
    for (int i = 1; i <= Nchemo; i++) {
        coordinate_x << to_string(i) << " ";    //The first line of file gives us the name of molecules//
        coordinate_y << to_string(i) << " ";    //The first line of file gives us the name of molecules//
    }
    coordinate_x << endl;
    coordinate_y << endl;


    //We write the coordinates of molecules during the first iteration
    coordinate_x << 0 << " ";                   //The first column of file gives us the time//
    coordinate_y << 0 << " ";                   //The first column of file gives us the time//
    for (int i = 1; i <= Nchemo; i++) {
        coordinate_x << chemo_vector[i].x << " ";
        coordinate_y << chemo_vector[i].y << " ";
    }
    coordinate_x << endl;
    coordinate_y << endl;


    //We save information given by receptors
    fstream nbr_absorption;

    nbr_absorption.open("data/nbr_absorption.txt", ios::out);
    nbr_absorption << "time/receptor ";
    for (int i = 1; i <= Nchemo; i++)
        nbr_absorption << to_string(i) << " ";    //The first line of file gives us the name of receptor//
    nbr_absorption << endl;

    //We write the coordinates of molecules during the first iteration
    nbr_absorption << 0 << " ";                   //The first column of file gives us the time//
    for (int i = 1; i <= Nrec; i++) {
//        nbr_absorption << 0 << " ";
        nbr_absorption << receptor_vector[i].n << " ";
    }
    nbr_absorption << endl;




    //We initialize the time of our simulation, and execute the simulation for 10 mins:
    for (double t=dt; t<=time_max; t+=dt) {
//        cout << "Progression : " << 100 * t / time_max << " %" << std::endl;

        //We are in an iteration of time

        //For each receptor:
        for(int i = 0; i < Nrec; i++)
            //We test if it absorbs Rcell molecule
            receptor_vector[i].absorption(chemo_vector);



        //For each molecule:
        for(int i = 0; i < Nchemo; i++) {

            //If Rcell molecule has been absorbed by Rcell recetor duriing the last iteration, we send it far from the bacterium
            chemo_vector[i].absorbed(receptor_vector, Rrec, Rcell, L);

            //We compute the new velocity while the molecule doesn't enter the bacterium
            do {
                chemo_vector[i].diffusion_langevin(eta, dt);
            } while (chemo_vector[i].in_the_cell(Rcell, L));

            //We update the position of the molecule
            chemo_vector[i].update_position(dt);

            //We keep it on the surface
            chemo_vector[i].boundary_conditions(L);
        }

        //We write the coordinates of molecules for this iteration
        coordinate_x << t << " ";                   //The first column of file gives us the time//
        coordinate_y << t << " ";                   //The first column of file gives us the time//
        for (int i = 1; i <= Nchemo; i++) {
            coordinate_x << chemo_vector[i].x << " ";
            coordinate_y << chemo_vector[i].y << " ";
        }
        coordinate_x << endl;
        coordinate_y << endl;

        //We write the number of molecules absorbed by each receptor until this iteration
        nbr_absorption << t << " ";                   //The first column of file gives us the time//
        for (int i = 1; i <= Nchemo; i++)
            nbr_absorption << receptor_vector[i].n << " ";
        nbr_absorption << endl;

    }

    //We close our files to save data
    coordinate_x.close();
    coordinate_y.close();
    nbr_absorption.close();

    return 0;
}