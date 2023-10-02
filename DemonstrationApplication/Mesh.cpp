#include "Common.h"
#include "Mesh.h"

Mesh::Mesh() {
	m_vao = NULL;
	m_vbo = NULL;
}

Mesh::~Mesh() { }

void Mesh::AddVertex(Vertex vertex) {
	m_vertices.push_back(vertex);
}

void Mesh::Create() {
	// Create and bind VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Create and bind VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);


	// Uploading the data

	vector<BYTE> m_data;
	//vbo.AddData(&v2, sizeof(glm::vec3));

	for (auto vertex : m_vertices) {
		m_data.insert(m_data.end(), (BYTE*)&vertex.position, (BYTE*)&vertex.position + sizeof(glm::vec3));
		m_data.insert(m_data.end(), (BYTE*)&vertex.texture, (BYTE*)&vertex.texture + sizeof(glm::vec2));
		m_data.insert(m_data.end(), (BYTE*)&vertex.normal, (BYTE*)&vertex.normal + sizeof(glm::vec3));
		m_data.insert(m_data.end(), (BYTE*)&vertex.colour, (BYTE*)&vertex.colour + sizeof(glm::vec3));
	}
	//GLsizei data_size = m_vertices.size() * (3 * sizeof(glm::vec3) + sizeof(glm::vec2));
	glBufferData(GL_ARRAY_BUFFER, m_data.size(), &m_data[0], GL_STATIC_DRAW);

	//	VERTEX BUFFERO OBJECT 1

	// Set the vertex attribute locations
	GLsizei istride = 3 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	// Colour vectors
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, istride, (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void Mesh::Render() {
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

// Release resources
void Mesh::Release()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

glm::vec3 Mesh::CalculateNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 norm;
	norm = glm::normalize(glm::cross(b - a, c - a));
	return norm;
}
