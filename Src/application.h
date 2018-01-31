// IFT3100H18_Triptyque/application.cpp
// Classe principale de l'application.

#pragma once

#include "ofMain.h"
#include "renderer.h"

class Application : public ofBaseApp
{
public:

  Renderer renderer;

  void setup();
  void draw();
  void exit();
};
