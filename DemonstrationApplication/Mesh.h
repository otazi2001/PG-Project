#pragma once
#include "Common.h"

class Mesh {
public:

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texture;
		glm::vec3 normal;
		glm::vec3 colour;
	};

	Mesh();
	~Mesh();

	void AddVertex(Vertex vertex);
	glm::vec3 CalculateNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);

	void Create();
	void Render();
	void Release();
	


private:
	vector<Vertex> m_vertices;
	UINT m_vbo;
	UINT m_vao;
};