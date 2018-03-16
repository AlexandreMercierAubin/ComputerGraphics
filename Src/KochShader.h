#pragma once
#include "AbstractShader.h"


class KochShader : public AbstractShader
{
	std::string fshader =
R"(
#version 430 core

in vec3 fragColor;

out vec4 color;

void main(void)
{
	color = vec4(fragColor, 1.0);
}


)";





	std::string vshader =
R"(#version 430 core      
                    
layout(location = 0) in vec3 in_vertice; 
layout(location = 1) in vec3 in_color;

uniform mat4 matModel;

out vec3 fragColor;

void main()
{

  gl_Position = matModel* vec4(in_vertice, 1.0);
  fragColor= in_color;

}
)";

public:
	virtual std::string getFragmentShader()
	{
		return fshader;
	}
	
	virtual std::string getVertexShader()
	{
		return vshader;
	}

};


