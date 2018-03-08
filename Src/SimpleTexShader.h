#pragma once
#include "AbstractShader.h"


class SimpleTexShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D text;

void main(void)
{
	color = texture(text, TexCoord);
}

)";




	std::string vshader =
R"(#version 430 core                          
in vec3 in_sommet;
in vec2 texCoord;

uniform mat4 matView;
uniform mat4 matPerspective;

out vec2 TexCoord;

void main()
{
	gl_Position = matPerspective*matView*vec4(in_sommet, 1.0);
	TexCoord = vec2(texCoord.x,texCoord.y);
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


