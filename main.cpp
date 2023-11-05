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
    
    //Def of parameters

    //Properties of the cell
    double a = 100;                           //radius//
    //in case of stirring
    double r1;                          //first radius of stirring//
    double r2;                          //second radius of stirring//

    //Properties of chemoattractants
    double r;                           //radius//
    double D1;                          //Diffusion coefficient in the volume//
    double D2;                          //Diffusion coefficient on the surface of the cell//
    double cinf;                        //Concentration at long distance//

    //Properties of receptors
    double s = 1;                           //radius//
    int N = 100;                              //Number of receptors//

    //Properties of the ambient medium
    double eta = 0;                     //viscosity of the medium//

    //time interval between two iterations
    double dt = 0.1;                    //for resolution of Langevin equation//   //10 iterations per second//

    
    //Definition of the surface of evolution of the system
    double Taillesurface = 2000;       //Taille de la surface sur l'axe des abscisses//



    //Creation of a vector of chemo
    vector<chemo> groupe;
    int n = cinf * Taillesurface * Taillesurface;         //We determine the number of chemo in the system thanks to their concentration//
    for(int i = 0; i < n; i++) {
        chemo C(Taillesurface, a, D1, D2);
        groupe.push_back(C);
    }

    //Creation of a vector of receptors (they are only points with two coordinates and size s)
    vector<receptor> detection_system;
    for(int i = 0; i < N; i++) {
        receptor R(s, a, 2.*M_PI*i/N);     //Receptors are first distributed around the bacterium//
        detection_system.push_back(R);
    }


    //We want to write our data in a file to exploit it in python

    //We save the position of particles, one for x coordinates, one for y coordinates
    fstream coordinate_x;
    fstream coordinate_y;

    coordinate_x.open("data/x.txt", ios::out);
    coordinate_x << "time/molecule ";
    coordinate_y.open("data/y.txt", ios::out);
    coordinate_y << "time/molecule ";
    for (int i = 1; i <= n; n++) {
        coordinate_x << to_string(i) << " ";    //The first line of file gives us the name of molecules//
        coordinate_y << to_string(i) << " ";    //The first line of file gives us the name of molecules//
    }
    coordinate_x << endl;
    coordinate_y << endl;


    //We write the coordinates of molecules during the first iteration
    coordinate_x << 0 << " ";                   //The first column of file gives us the time//
    coordinate_y << 0 << " ";                   //The first column of file gives us the time//
    for (int i = 1; i <= n; n++) {
        coordinate_x << groupe[i].x << " ";     
        coordinate_y << groupe[i].y << " ";    
    }
    coordinate_x << endl;
    coordinate_y << endl;


    //We save information give by receptors
    fstream nbr_absorption;

    nbr_absorption.open("data/nbr_absorption.txt", ios::out);
    nbr_absorption << "time/receptor ";
    for (int i = 1; i <= n; n++)
        nbr_absorption << to_string(i) << " ";    //The first line of file gives us the name of receptor// 
    nbr_absorption << endl;

    //We write the coordinates of molecules during the first iteration
    nbr_absorption << 0 << " ";                   //The first column of file gives us the time//
    for (int i = 1; i <= n; n++) {
        nbr_absorption << detection_system[i].n << " ";        
    }
    nbr_absorption << endl;



    //We initialize the time of our simulation, and execute the simulation for 10 mins:
    for (double t=dt; t<=600; t+=dt) {


        //We are in an iteration of time

        //For each receptor:
        for(int i = 0; i < N; i++)
            //We test if it absorbs a molecule
            detection_system[i].absorption(groupe);
        

        //For each molecule:
        for(int i = 0; i < n; i++) {

            //If a molecule has been absorbed by a recetor duriing the last iteration, we send it far from the bacterium
            groupe[i].absorbed(detection_system, s, a, Taillesurface);

            //We compute the new velocity while the molecule doesn't enter the bacterium
            do {
                groupe[i].diffusion_langevin(eta, dt);
            } while (groupe[i].in_the_cell(a, Taillesurface));

            //We update the position of the molecule
            groupe[i].update_position(dt);

            //We keep it on the surface
            groupe[i].boundary_conditions(Taillesurface);
        }
    

        //We write the coordinates of molecules for this iteration
        coordinate_x << t << " ";                   //The first column of file gives us the time//
        coordinate_y << t << " ";                   //The first column of file gives us the time//
        for (int i = 1; i <= n; n++) {
            coordinate_x << groupe[i].x << " ";     
            coordinate_y << groupe[i].y << " ";    
        }
        coordinate_x << endl;
        coordinate_y << endl;

        //We write the number of molecules absorbed by each receptor until this iteration
        nbr_absorption << t << " ";                   //The first column of file gives us the time//
        for (int i = 1; i <= n; n++)
            nbr_absorption << detection_system[i].n << " ";         
        nbr_absorption << endl;

    }

    //We close our files to save data
    coordinate_x.close();
    coordinate_y.close();
    nbr_absorption.close();

    return 0;
}