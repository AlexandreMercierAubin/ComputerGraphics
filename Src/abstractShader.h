#pragma once
#include <string>

class AbstractShader
{
	public:
		virtual std::string getVertexShader() 
		{
			return shader;
		}
		virtual std::string getFragmentShader() 
		{
			return shader;
		}
		
	private:
		std::string shader="";
};
