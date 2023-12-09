#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<C:/mingw32/include/SFML/Graphics.hpp>
#include<C:/mingw32/include/opencv2/opencv.hpp>

using namespace std;

int main() {

    // Ouverture du fichier en lecture
    ifstream parameters("data/param.csv");

    // Lecture du fichier CSV line par line
    string line;

    // Vecteur pour stocker les valeurs de la ligne
    vector<string> Parameters;
    vector<double> valeursParameters;

    while (getline(parameters, line)) {

        // Utilisation d'un stringstream pour séparer les valeurs par des virgules
        stringstream ss(line);
        string donnee;

        // Lecture des valeurs séparées par des virgules et conversion en double
        while (getline(ss, donnee, ',')) {
            try {
                double valeurParameters = stod(donnee);
                valeursParameters.push_back(valeurParameters);
            } catch (const invalid_argument& e) {
                string Parameter = donnee;
                Parameters.push_back(Parameter);
            }
        }
    }


    //Affichage du tableau crée à partir de param.csv

    for (const auto& noms : Parameters) {
        cout << noms << " ";
    }
    cout << endl;


    double Rcell = valeursParameters[0];
    int Rrec = valeursParameters[1];
    int Nrec = valeursParameters[2];
    int Nchem = valeursParameters[3];
    double time_max = valeursParameters[4];
    double dt = valeursParameters[5];
    int L = valeursParameters[6];

    //Ecriture des coordonnees des recepteurs dans un fichier
    ifstream recs("data/rec.txt");
    vector<double> receptors;

    while (getline(recs, line)) {

        // Utilisation d'un stringstream pour séparer les valeurs par des virgules
        stringstream ss(line);
        string Rec;

        // Lecture des valeurs séparées par des virgules et conversion en double
        while (getline(ss, Rec, ' ')) {
            double coord_Rec = stod(Rec);
            receptors.push_back(coord_Rec);
        }
    }

    //Analyse de x.txt et y.txt
    ifstream coord_x("data/x.txt");
    ifstream coord_y("data/y.txt");

    string line_x, line_y;
    vector<double> x, y;

    string coordinate_x;
    double value_x;

    string coordinate_y;
    double value_y;

    //cv::VideoWriter videoWriter;
    //videoWriter.open("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(L, L));

    //We declare the SFML window
    sf::RenderWindow window(sf::VideoMode(L,L), "Image initiale", sf::Style::Default);

    double t = 0.;

    //The simulation will run until the the window is closed
    while ((window.isOpen()) && (getline(coord_x, line_x)) && (getline(coord_y, line_y))) {

        // on inspecte tous les évènements de la fenêtre qui ont été émis depuis la précédente itération
        sf::Event event;
        while (window.pollEvent(event)) {

	        // évènement "fermeture demandée" : on ferme la fenêtre en appuyant sur close
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // effacement de la fenêtre en noir
        window.clear(sf::Color::Black);

        stringstream ssx(line_x);
        stringstream ssy(line_y);
        // Lecture des valeurs séparées par des virgules et conversion en double
        while (getline(ssx, coordinate_x, ' ')) {
            value_x = stod(coordinate_x);
            x.push_back(value_x);
        }

        // Lecture des valeurs séparées par des virgules et conversion en double
        while (getline(ssy, coordinate_y, ' ')) {
            value_y = stod(coordinate_y);
            y.push_back(value_y);
        }
      
        for (unsigned int i=0; i < x.size(); i++) {
            // création d'un cercle pour dessiner les chlamys
            sf::CircleShape chemo(1.);
            chemo.setFillColor(sf::Color(100,255,255,255));
            chemo.setPosition(x[i]-1.,y[i]-1.);
            window.draw(chemo);
        }

        sf::CircleShape cell(Rcell);
        cell.setFillColor(sf::Color(255,100,255,255));
        cell.setPosition(L/2.-Rcell,L/2.-Rcell);
        window.draw(cell);

        for(int i=0; i<Nrec; i++) {
            sf::CircleShape Rec(Rrec);
            Rec.setFillColor(sf::Color(255,255,100,255));
            Rec.setPosition(receptors[2*i]-Rrec/2., receptors[2*i+1]-Rrec/2.);
            window.draw(Rec);
        }

        x.clear();
        y.clear();

        // fin de la frame courante, affichage de tout ce qu'on a dessiné
        window.display();

        // Capture de la fenêtre SFML
        //sf::Image screenshot = window.capture();

        // Obtention des dimensions de l'image SFML
        //sf::Vector2u imageSize = screenshot.getSize();

        // Création d'une matrice OpenCV avec les mêmes dimensions et le même type
        //cv::Mat imageMat(imageSize.y, imageSize.x, CV_8UC4);

        // Copie des données des pixels SFML dans la matrice OpenCV
        //memcpy(imageMat.data, screenshot.getPixelsPtr(), imageSize.x * imageSize.y * 4);

        // Enregistrement de l'image dans la vidéo OpenCV
        //video.write(imageMat);

        t += dt;
    }

    // Fermeture du fichier
    parameters.close();
    recs.close();
    coord_x.close();
    coord_y.close();

    // Fermeture de la vidéo OpenCV
    //video.release();

    return 0;
}