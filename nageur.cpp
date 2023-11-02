#include<iostream>
#include<ostream>
#include<cmath>
#include<random>
#include"fonctions.h"
#include"chlamy.h"

using namespace std;

//Initialisation de la graine des processus aléatoires
random_device rdc;
default_random_engine engc(rdc());
uniform_real_distribution<float> distc(0., 1.);

//Constructeurs de la classe chlamy

chlamy::chlamy() {
  x = 0;
  y = 0;
  vx = 0;
  vy = 0;
  v0 = 1;
  Taille = 9;
}

chlamy::chlamy(double Taillesurfacex, double Taillesurfacey, double vmoy, double vstd) {
  Taille = rejetgaussien(9.,1.);
  positioninitiale(Taillesurfacex, Taillesurfacey);
  vitesseinitiale(vmoy, vstd);
}


//Destructeur de la classe chlamy
chlamy::~chlamy() {
}

//On initialise les positions initiales des oiseaux
void chlamy::positioninitiale(double Taillesurfacex, double Taillesurfacey) {
  //Distribution uniforme de la position des chlamy si il n'y a pas d'obstacle

  x = 6*Taillesurfacex * distc(engc) - 17.*Taillesurfacex/3.;
  y = (Taillesurfacey-2*Taille) * distc(engc) + Taille;
}



//On initialise les vitesses initiales des oiseaux
void chlamy::vitesseinitiale(double vmoy, double vstd) {
  double normvitesse;

  //La distribution initiale des vitesses et gaussiennes pour choisir la direction de l'oiseau aléatoirement
  v0 = rejetgaussien(vmoy, vstd);
  vx = rejetgaussien(0, vstd);
  vy = rejetgaussien(0, vstd);
  normvitesse = norm(vx, vy);

  //On normalise la vitesse initiale à la vitesse du groupe
  vx = v0 * vx/normvitesse;
  vy = v0 * vy/normvitesse;
}




//Cette fonction permettra d'être sûr qu'aucune chlamy ne pénétrera un mur
bool chlamy::enceinte(double Tailleporte, double Angleporte, double Taillesurfacex, double Taillesurfacey) {
  
  //On a six murs dans la simulation, on calcule la distance de la chlamy à chacun de ces murs
  //On différencie néanmoins si la chlamy est à gauche ou à droite de la porte pour faciliter le calcul

  if (y+vy - Taille/(2*sin(Angleporte)) - (Taillesurfacey-Tailleporte)/2 <= -fabs(x+vx-Taillesurfacex/2)/tan(Angleporte))
    return false;  

  else if (y+vy + Taille/(2*sin(Angleporte)) - (Taillesurfacey+Tailleporte)/2 >= fabs(x+vx-Taillesurfacex/2)/tan(Angleporte))
    return false; 

  else
    return true;
}


void chlamy::reorientation(double moyenne, double largeur) {

  double theta;

  //L'angle theta est modifié afin que les chlamys se reorientent en fuyant la lumière
  theta = distribution_lorentzienne(moyenne, largeur);

  //v0 = v0 + rejetgaussien(0, v0/100.);
  vx = v0 * cos(theta);
  vy = v0 * sin(theta);
}



void chlamy::diffusion_angulaire(double Dr) {

  //On ajoute à l'angle theta un petit angle dtheta à chaque itération. C'est une diffusion angulaire.
  double theta = atan2(vy,vx) + rejetgaussien(0, 2*Dr);
  vx = v0*cos(theta);
  vy = v0*sin(theta);
}



void chlamy::non_collision (vector<chlamy> groupe, double k) {

  double dmin = v0;
  unsigned int imin = 0;

  for (unsigned int i= 0; i < groupe.size(); i++) {
    if ((norm(groupe[i].x-x, groupe[i].y-y) < (groupe[i].Taille+Taille)/2) && (norm(groupe[i].x-x, groupe[i].y-y) > 0)) {
      double distance = norm(groupe[i].x-x, groupe[i].y-y);
      vx = vx - k/(groupe[i].x - x - vx)*distance; 
      vy = vy - k/(groupe[i].y - y - vy)*distance;
      double vitesse = norm(vx, vy);
      vx = v0 * vx/vitesse;
      vy = v0 * vy/vitesse;
    }
  }

  for (unsigned int i = 0; i < groupe.size(); i++) {

    double Distance1 = fabs(vy*(groupe[i].x-x) + vx*(y-groupe[i].y))/v0;
    double Distance2 = norm(groupe[i].x - x, groupe[i].y -y);
    
    if ((Distance1 < (groupe[i].Taille + Taille)/2) && (Distance2 > Taille/2) && (sqrt(Distance2*Distance2 - Distance1*Distance1) < dmin)) {
      dmin = sqrt(Distance2*Distance2 - Distance1*Distance1);
      imin = i;
    }
  }

  if (dmin < v0) {

    double Distance1 = fabs(vy*(groupe[imin].x-x) + vx*(y-groupe[imin].y))/norm(vx, vy);
    double Distance2 = norm(groupe[imin].x - x, groupe[imin].y -y);

    double Distance3 = sqrt(Distance2*Distance2 - Distance1*Distance1);
    vx = vx/v0 * Distance3;
    vy = vy/v0 * Distance3;
  }
}

void chlamy::tirage_uniforme() {
  double theta = 4*M_PI*distc(engc) - 2*M_PI;
  vx = v0*cos(theta);
  vy = v0*sin(theta);
}