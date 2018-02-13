#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include<fstream>
#include <experimental/filesystem>

namespace Core
{
	class Shader_Loader
	{
	private:
		std::string ReadShader(char *filename);
		GLuint CreateShader(GLenum shaderType, std::string source, char* shadername);
	
	public:
		Shader_Loader(void);
		~Shader_Loader(void);

		GLuint CreateProgram(char* VertexShaderFilename, char* FragmentShaderFilename);
	};
}


