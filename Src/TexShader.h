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
	color = texture(text, TexCoord);
}

)";




	std::string vshader =
R"(#version 430 core                          
in vec3 position;
in vec2 texCoord;

uniform mat4 matView;
uniform mat4 matTranslation;
uniform mat4 matPerspective;
uniform mat4 matRotation;
uniform mat4 matScale;

out vec2 TexCoord;

void main()
{
	gl_Position =  matPerspective*matView*matTranslation*matRotation*matScale*(vec4(position, 1.0));
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


