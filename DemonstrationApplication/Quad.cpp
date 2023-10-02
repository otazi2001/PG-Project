#include "Mesh.h"

class Quad : public Mesh {
public:
	void Create(float width, float height) {
		glm::vec3 vertex1(-1.0f * width, 1.0f * height, 0.0f);  // Top-left vertex
		glm::vec3 vertex2(-1.0f * width, -1.0f * height, 0.0f); // Bottom-left vertex
		glm::vec3 vertex3(1.0f * width, -1.0f * height, 0.0f);  // Bottom-right vertex
		glm::vec3 vertex4(1.0f * width, 1.0f * height, 0.0f);

		AddVertex({ vertex1,		glm::vec2(0.f, 1.f),  		CalculateNormal(vertex1, vertex2, vertex3),   		glm::vec3(0.f)});
		AddVertex({ vertex2,		glm::vec2(0.f, 0.f),  		CalculateNormal(vertex1, vertex2, vertex3),   		glm::vec3(0.f) });
		AddVertex({ vertex3,		glm::vec2(1.f, 0.f),  		CalculateNormal(vertex1, vertex2, vertex3),   		glm::vec3(0.f) });

		AddVertex({ vertex1,		glm::vec2(0.f, 1.f),  		CalculateNormal(vertex1, vertex2, vertex3),   		glm::vec3(0.f) });
		AddVertex({ vertex3,		glm::vec2(1.f, 0.f),  		CalculateNormal(vertex1, vertex2, vertex3),   		glm::vec3(0.f) });
		AddVertex({ vertex4,		glm::vec2(1.f, 1.f),  		CalculateNormal(vertex1, vertex2, vertex3),   		glm::vec3(0.f) });
		Mesh::Create();
	}
};