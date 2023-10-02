#pragma once
#include "Common.h"
#include "Mesh.h"
#include "Shader.h"

class GameObject {
public:
	void Create(const Mesh& mesh, GLuint TexID);
	void Create(const Mesh& mesh, string TexPath, bool Wrap);

	void		setModelMatrix(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	glm::mat4	ModelMatrix();

	void Render(Shader* shader);

private:
	Mesh m_mesh;
	GLuint m_texID;

	glm::mat4 m_modelMat;
};