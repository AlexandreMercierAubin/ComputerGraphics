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
		std::string ReadShader(std::string filename);
		GLuint CreateShader(GLenum shaderType, std::string source, std::string shadername);
	
	public:
		Shader_Loader(void);
		~Shader_Loader(void);

		GLuint CreateProgram(std::string VertexShaderFilename, std::string FragmentShaderFilename);
	};
}


