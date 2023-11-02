#ifndef chlamy_H
#define chlamy_H

#include<ostream>
#include<vector>

using namespace std;

class porte;
class chlamy {
public:

  //Position et vitesse de la chlamy
  double x;
  double y;
  double vx;
  double vy;

  //Autres propriétés de la chlamy
  double v0; //vitesse moyenne de la chlamy
  double Taille; //taille de la chlamy

  //Constructeurs et destructeur de la classe
  chlamy();
  chlamy(double Taillesurfacex, double Taillesurfacey, double vmoy, double vstd);
  ~chlamy();

  //Premier comportement des chlamys
  bool enceinte(double Tailleporte, double Angleporte, double Taillesurfacex, double Taillesurfacey);
  void reorientation(double Taillesurfacex, double Taillesurfacey);
  void diffusion_angulaire(double Dr);
  void non_collision(vector<chlamy> groupe, double k);
  void tirage_uniforme();

  //Deuxième comportement des chlamys

  //Génération de la première frame
  void positioninitiale(double Taillesurfacex, double Taillesurfacey);
  void vitesseinitiale(double vmoy, double vstd);
  
  
};

#endif
