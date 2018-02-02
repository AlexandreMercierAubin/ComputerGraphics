// IFT3100H18_Triptyque/main.cpp
// Exemple d'échantillonnage dans une image chargée en mémoire pour en extraire 3 nouvelle images.
// Ces nouvelles images sont ensuite dessinées de gauche à droite avec un espacement horizontal et vertical.

#include "ofMain.h"
#include "application.h"

int main( )
{
  ofSetupOpenGL(512, 512, OF_WINDOW);
  ofRunApp(new Application());
}
