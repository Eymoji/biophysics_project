#include<iostream>
#include<random>
#include<time.h>
#include<cmath>
#include<vector>
#include<fstream>
#include<string>
#include<iomanip>
#include"fonctions.h"
#include"chlamy.h"

using namespace std;


int main() {

  //Initialisation de la graine des processus aléatoires
  random_device rd;
  default_random_engine eng(rd());
  uniform_real_distribution<float> dist(0., 1.);


  //Variables et paramètres temporels de la simulation
  int t = 0;
  int framerate = 20;  
  int frame_par_fichier = 500;
  int total_frame = 10000;

  //Définition de la taille de la simulation
  double Taillesurfacex = 1600;
  double Taillesurfacey = 500;

  //Définition de la porte
  double Tailleporte = 10, Angleporte = M_PI/4;
  
  //Définition de la vitesse moyenne  des chlamys en adaptant au nombre de frame par seconde
  double vmoy = 77.29/framerate;
  double vstd = 26.64/framerate;

  //Initialisation du groupe de chlamy
  vector<chlamy> groupe;

  //Nombre de chlamy dans le groupe
  int N = 8000;

  //Coefficient de diffusion angulaire
  double Dr = 0.81/framerate;

  //Taux de changement de direction des chlamys
  double alpha = 9.9/framerate;
  double largeur = 0.325;

  //Constante de ressort de répulsion entre chlamys
  double k = 0.4;


  //On ouvre un fichier pour stocker des informations sur le programme
  fstream fich_parametre;

  fich_parametre.open("data_simulations/N10000D10V90_theil/parametres.txt", ios::out);
  fich_parametre << "Total_frame frame_par_fichier framerate Taille_x Taille_y Taille_porte Angle_Porte Vitesse_moyenne N alpha Dr k" << endl << total_frame << " " << frame_par_fichier <<" " << framerate << " " << Taillesurfacex << " " << Taillesurfacey << " " << Tailleporte << " " << Angleporte << " " << vmoy << " " << N << " " << alpha << " " << Dr << " " << k << endl;
  fich_parametre.close();

  
  vector<fstream> x;
  vector<fstream> y;
  for (int i = 0; i < total_frame/frame_par_fichier; i++) {
    x.push_back(fstream());
    x[i].open("data_simulations/N10000D10V90_theil/fichier_x_" + to_string(i) + ".txt", ios::out);
    x[i] << "t ";
    for (int j = 0; j < N; j++) {
      x[i] << to_string(j) << " ";
    }
    x[i] << endl;
  }

  for (int i = 0; i < total_frame/frame_par_fichier; i++) {
    y.push_back(fstream());
    y[i].open("data_simulations/N10000D10V90_theil/fichier_y_" + to_string(i) + ".txt", ios::out);
    y[i] << "t ";
    for (int j = 0; j < N; j++) {
      y[i] << to_string(j) << " ";
    }
    y[i] << endl;
  }

  
  for(int i = 0; i < N; i++) {
    chlamy C(Taillesurfacex, Taillesurfacey, vmoy, vstd);
    groupe.push_back(C);
  }


  //On fait tourner le programme pendant 10000 frames, par groupe de 200 pour l'ecriture dans les fichiers
  for (int fichier = 0; fichier < total_frame/frame_par_fichier; fichier++) {
    for (int frame = 0; frame < frame_par_fichier; frame++) {


    
      x[fichier] << double(frame_par_fichier*fichier + frame)/framerate << " ";
      for (int j = 0; j < N; j++) {
        x[fichier] << groupe[j].x << " ";
      }
      x[fichier] << endl;
    

      y[fichier] << double(frame_par_fichier*fichier + frame)/framerate << " ";
      for (int j = 0; j < N; j++) {
        y[fichier] << groupe[j].y << " ";
      }
      y[fichier] << endl;


      t+=1;
    
      //Premier comportement des chlamys
      for (int i = 0; i < N; i++) {

        double p = dist(eng);

        //Pour représenter la persistence, on inclue une probabilité de modifier la vitesse d'une chlamy
        if (p < alpha)
	        groupe[i].reorientation(0, largeur);
        if ((groupe[i].enceinte(Tailleporte, Angleporte, Taillesurfacex, Taillesurfacey) == false) & (p < alpha))
          groupe[i].tirage_uniforme();
        groupe[i].diffusion_angulaire(Dr);
        groupe[i].non_collision(groupe, k);
      }


      //Mise à jour de la position des oiseaux
      for (int i = 0; i < N; i++) {

        if ((groupe[i].enceinte(Tailleporte, Angleporte, Taillesurfacex, Taillesurfacey) == true)) {
	        groupe[i].x = groupe[i].x + groupe[i].vx; //position selon x
	        groupe[i].y = groupe[i].y + groupe[i].vy; //position selon y
        }

        //condition périodique de la surface SFML selon xx;
        if (groupe[i].x > Taillesurfacex)
	        groupe[i].x -= 21*Taillesurfacex;

        //condition périodique de la surface SFML selon y
        if (groupe[i].y < 0)
	        groupe[i].y += Taillesurfacey;
        else if (groupe[i].y > Taillesurfacey)
	        groupe[i].y -= Taillesurfacey;
      }
    }

    x[fichier].close();
    y[fichier].close();
  }

  return 0;
}