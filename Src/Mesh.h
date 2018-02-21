#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <assimp\types.h>
#include <glm\gtc\matrix_transform.hpp>


class Mesh {
public:

	struct Vertex {
		// Position
		glm::vec3 Position;
		// Normal
		glm::vec3 Normal;
		// TexCoords
		glm::vec2 TexCoords;
	};

	struct Texture {
		GLuint id;
		string type;
		aiString path;
	};

	/*  Mesh Data  */
	vector<Vertex> verticesModel;
	vector<GLuint> indicesModel;
	vector<Texture> texturesModel;

	/*  Functions  */
	// Constructor
	Mesh(vector<Vertex> *vertices, vector<GLuint> *indices, vector<Texture> *textures);

	// Render the Mesh
	void Draw(GLuint shader);

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh();

};





