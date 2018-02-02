// IFT3100H18_Triptyque/renderer.cpp
// Classe responsable du rendu de l'application.

#include "renderer.h"

void Renderer::setup()
{
  ofSetFrameRate(0);

  // définir la valeur de l'espacement vertical et horizontal
  offset_vertical = 32;
  offset_horizontal = 32;

  // définir la résolution des images de destination
  image_width = 256;
  image_height = 256;

  

  // importer les images source
  for (int i = 0; i < imageNameArray.size(); i++) 
  {
	  ofImage image;
	  image.load(imageNameArray[i]);
	  images.push_back(image);
  }

  double squared = sqrt(imageNameArray.size());
  imageboxX = ceil(squared);
  imageboxY = floor(squared);

  image_out_width = imageboxX *image_width + imageboxX *offset_vertical;
  image_out_height = imageboxY *image_height + imageboxY *offset_horizontal;


  // ajuster la résolution de la fenêtre en fonction de la résolution de l'image source et des espacements
  ofSetWindowShape(
	  image_out_width + offset_horizontal * 2,
	  image_out_height + offset_vertical * 2);

}

void Renderer::draw()
{
  ofClear(0);


  // dessiner l'image
  for (int i = 0; i < imageNameArray.size(); i++)
  {
	  int x = i % imageboxX;
	  int y = i / imageboxX;

	  images[i].draw(
		  offset_horizontal * (x+1) + image_width*x,
		  offset_vertical * (y + 1)+ image_height*y,
		  image_width,
		  image_height);
  }

  image_out.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
  image_out.save("out.jpg");
}
