// IFT3100H18_Triptyque/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"
#include <vector>
#include <math.h>

class Renderer
{
public:

  ofImage image_out;

  std::vector<ofImage> images;
  std::vector<string> imageNameArray = { "PTV.jpg", "SetItOff.jpg","green-day-21st-century.jpg","MCR.jpg","screamingbloodymurder.jpg" };

  int image_width;
  int image_height;
  int image_out_width;
  int image_out_height;

  int offset_vertical;
  int offset_horizontal;

  int imageboxX;
  int imageboxY;

  void setup();
  void draw();
};
