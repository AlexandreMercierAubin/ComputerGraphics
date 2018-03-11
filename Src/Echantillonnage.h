#pragma once
#include"PixelAccess.h"

void SurfaceSampling(SDL_Surface *image, string texturePath) {
	string sPercentage = texturePath.substr(0, (texturePath.find("|")));
	texturePath = texturePath.substr((texturePath.find("|")) + 1, string::npos);
	string sPosition = texturePath.substr(0, (texturePath.find("|")));
	texturePath = texturePath.substr((texturePath.find("|")) + 1, string::npos);

	int percentage = atoi(sPercentage.c_str());
	int position = atoi(sPosition.c_str());
	SDL_Surface *imageEchantillon = Model::loadImage(texturePath);
	int w = ((min(image->w, imageEchantillon->w) * percentage) / 100);
	int h = ((min(image->h, imageEchantillon->h)* percentage) / 100);
	;
	SDL_LockSurface(image);

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++) {
			if (position == 3)
				putpixel(image, i + (image->w / 2), j + (image->h / 2), getpixel(imageEchantillon, i, j));
			else if (position == 2)
				putpixel(image, i, j + (image->h / 2), getpixel(imageEchantillon, i, j));
			else if (position == 1)
				putpixel(image, i + (image->w / 2), j, getpixel(imageEchantillon, i, j));
			else
				putpixel(image, i, j, getpixel(imageEchantillon, i, j));
		}
	}
	SDL_FreeSurface(imageEchantillon);
	SDL_UnlockSurface(image);
};