#include "Mesh.h"

class Cube : public Mesh {
public:
	void Create(float wlh, glm::vec3 color) {
		glm::vec3 vert1(-wlh / 2,	wlh / 2,	-wlh / 2);
		glm::vec3 vert2(-wlh / 2,	-wlh / 2,	-wlh / 2);
		glm::vec3 vert3(wlh / 2,	-wlh / 2,	-wlh / 2);
		glm::vec3 vert4(wlh / 2,	wlh / 2,	-wlh / 2);

		glm::vec3 vert5(wlh / 2,	wlh / 2,	wlh / 2);
		glm::vec3 vert6(wlh / 2,	-wlh / 2,	wlh / 2);
		glm::vec3 vert7(-wlh / 2,	-wlh / 2,	wlh / 2);
		glm::vec3 vert8(-wlh / 2,	wlh / 2,	wlh / 2);


		// front
		AddVertex({ vert1,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert1, vert2, vert4),   		color});
		AddVertex({ vert2,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert1, vert2, vert4),   		color });
		AddVertex({ vert4,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert1, vert2, vert4),   		color });

		AddVertex({ vert2,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert1, vert2, vert4),   		color });
		AddVertex({ vert3,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert1, vert2, vert4),   		color });
		AddVertex({ vert4,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert1, vert2, vert4),   		color });

		// right
		AddVertex({ vert4,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert4, vert3, vert5),   		color });
		AddVertex({ vert3,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert4, vert3, vert5),   		color });
		AddVertex({ vert5,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert4, vert3, vert5),   		color });

		AddVertex({ vert3,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert4, vert3, vert5),   		color });
		AddVertex({ vert6,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert4, vert3, vert5),   		color });
		AddVertex({ vert5,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert4, vert3, vert5),   		color });

		// back
		AddVertex({ vert5,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert5, vert6, vert8),   		color });
		AddVertex({ vert6,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert5, vert6, vert8),   		color });
		AddVertex({ vert8,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert5, vert6, vert8),   		color });

		AddVertex({ vert6,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert5, vert6, vert8),   		color });
		AddVertex({ vert7,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert5, vert6, vert8),   		color });
		AddVertex({ vert8,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert5, vert6, vert8),   		color });

		// left
		AddVertex({ vert8,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert7, vert1),   		color });
		AddVertex({ vert7,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert7, vert1),   		color });
		AddVertex({ vert1,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert7, vert1),   		color });

		AddVertex({ vert7,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert7, vert1),   		color });
		AddVertex({ vert2,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert7, vert1),   		color });
		AddVertex({ vert1,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert7, vert1),   		color });

		// top
		AddVertex({ vert8,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert1, vert5),   		color });
		AddVertex({ vert1,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert1, vert5),   		color });
		AddVertex({ vert5,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert1, vert5),   		color });

		AddVertex({ vert1,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert1, vert5),   		color });
		AddVertex({ vert4,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert1, vert5),   		color });
		AddVertex({ vert5,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert8, vert1, vert5),   		color });

		// bottom
		AddVertex({ vert2,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert2, vert7, vert3),   		color });
		AddVertex({ vert7,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert2, vert7, vert3),   		color });
		AddVertex({ vert3,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert2, vert7, vert3),   		color });

		AddVertex({ vert7,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert2, vert7, vert3),   		color });
		AddVertex({ vert6,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert2, vert7, vert3),   		color });
		AddVertex({ vert3,		glm::vec2(0.f, 0.f),  		CalculateNormal(vert2, vert7, vert3),   		color });


		Mesh::Create();
	}

};