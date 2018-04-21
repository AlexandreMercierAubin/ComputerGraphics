#pragma once
#include "AbstractShader.h"


class TexShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D text;
uniform vec4 vColor;

void main(void)
{
	color = texture(text, TexCoord)*vColor;
}

)";




	std::string vshader =
R"(#version 430 core                          
in vec3 position;
in vec2 texCoord;

uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;

out vec2 TexCoord;

void main()
{
	gl_Position =  matProjection*matView*matModel*(vec4(position, 1.0));
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


