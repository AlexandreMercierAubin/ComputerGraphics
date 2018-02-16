#pragma once
#include "abstractShader.h"


class kochFragmentShader : public abstractShader
{
	std::string fshader =
R"(#version 430 core

in vec3 fragColor;

out vec4 color;

void main(void)
{
	color = vec4(fragColor, 1.0);
}


)";

public:
	virtual std::string getShader()
	{
		return fshader;
	}

};


