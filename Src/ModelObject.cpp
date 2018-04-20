#pragma once
#include "ModelObject.h"



void ModelObject::Create(GLuint &Program)
{
	name = "Modele (" + modelPath + ")";

	program = Program;

	//default settings
	scale = glm::vec3(0.0028f, 0.0028f, 0.0028f);
	model = Model(modelPath);

}

void ModelObject::Draw(glm::mat4 &projection, glm::mat4 &view , glm::vec3 &camPos, const vector<Light*>& lights)
{

	glUseProgram(program);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatProjection = glGetUniformLocation(program, "matProjection");
	glUniformMatrix4fv(MatProjection, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 r;
	AbstractObject::MatRotationDegree(program, r, rotationDegree);
	glm::mat4 t;
	AbstractObject::MatTranslation(program, t, position);
	glm::mat4 s;
	AbstractObject::MatScale(program, s, scale);

	glm::mat4 mModel = t * r* s;

	GLuint MatModel = glGetUniformLocation(program, "matModel");
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, &mModel[0][0]);

	AbstractObject::uniformColor(program, color);
	AbstractObject::uniformLight(program, lights);
	AbstractObject::uniformCameraPosition(program, camPos);
	AbstractObject::uniformShininess(program,shininess);

	model.Draw(program);
}

void ModelObject::setModelToCreate(string path)
{
	modelPath = path;
	name = "Modele (" + path + ")";
}

bool ModelObject::raycast(const Ray &ray, double &distance, glm::vec3 &normal, std::shared_ptr<AbstractObject> &object)
{
	bool intersect = false;

	glm::mat4 r;
	AbstractObject::MatRotationDegree(program, r, rotationDegree);
	glm::mat4 t;
	AbstractObject::MatTranslation(program, t, position);
	glm::mat4 s;
	AbstractObject::MatScale(program, s, scale);

	glm::mat4 mModel = t * r* s;

	// Boucler sur tous les mesh
	for (Mesh mesh : model.getMeshes())
	{
		// Boucler sur tous les triangles
		for (unsigned int i = 0; i < mesh.indicesModel.size(); i += 3)
		{
			// Basé sur algo notes de cours p.167

			// Sommets du triangle
			glm::vec3 p[3] = {
				glm::vec3(mModel * glm::vec4(mesh.verticesModel.at(mesh.indicesModel.at(i)).Position, 1.0f)),
				glm::vec3(mModel * glm::vec4(mesh.verticesModel.at(mesh.indicesModel.at(i + 1)).Position, 1.0f)),
				glm::vec3(mModel * glm::vec4(mesh.verticesModel.at(mesh.indicesModel.at(i + 2)).Position, 1.0f))
			};

			// Plan associé à ce triangle
			glm::vec3 n = glm::cross(p[1] - p[0], p[2] - p[0]);
			n /= glm::length(n);

			// Vérifier que le rayon entre en collision avec le plan
			if (glm::dot(ray.direction, n) >= 0)
				continue;

			// Calculer distance entre l'origine du rayon et le plan
			double dist = glm::dot(n, ray.origin) + glm::dot(-n, p[0]);
			if (dist <= 0.0)
				continue; // Plan derrière le rayon

			// Point d'intersection entre le rayon et le plan
			glm::vec3 intersection = ray.origin + glm::vec3(ray.direction.x * dist, ray.direction.y * dist, ray.direction.z * dist);

			// Vérification que l'intersection est à l'intérieur des bornes du triangle
			bool inside = true;
			int j = 0;
			while (inside && j < 3)
			{
				int nextJ = j == 2 ? 0 : j + 1;
				glm::vec3 edge = p[nextJ] - p[j];
				glm::vec3 direction = intersection - p[j];
				glm::vec3 perpendicular = glm::cross(edge, direction);

				if (glm::dot(n, perpendicular) < 0)
					inside = false; // Le rayon passe à l'extérieur du triangle

				++j;
			}

			if (inside)
			{
				intersect = true;
				distance = glm::min(distance, dist);
				normal = n;
			}
		}
	}

	return intersect;
}

