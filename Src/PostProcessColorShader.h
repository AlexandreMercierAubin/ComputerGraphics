#pragma once
#include "abstractShader.h"

class PostProcessColorShader : public AbstractShader
{
		std::string fshader =
			R"(#version 430 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D text;

void main(void)
{
	color = texture(text, TexCoord)*vec4(1,0.5,0.5,1);
}

)";




		std::string vshader =
			R"(#version 430 core                          
in vec2 position;
in vec2 texCoord;
out vec2 TexCoord;

void main()
{
	gl_Position =  vec4(position,0, 1.0);
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
