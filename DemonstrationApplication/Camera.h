#pragma once
#include "Common.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"

class Camera {
public:
	void Initialize(int width, int height);

	glm::mat4 GetPerspectiveProjMatrix();
	glm::mat4 GetOrthographicProjMatrix();
	glm::mat4 GetPerspectiveViewMatrix();
	glm::mat4 GetOrthographicViewMatrix();
	glm::mat3 GetNormalMatrix(glm::mat4 ViewMatrix);

	void Forwards(KeyboardEvent e);
	void Backwards(KeyboardEvent e);
	void StrafeLeft(KeyboardEvent e);
	void StrafeRight(KeyboardEvent e);

	void SetViewByMouse(MouseEvent e);
	
private:
	glm::vec3 position;
	glm::vec3 upVector;
	glm::vec3 frontVector;
	glm::vec3 strafeVector;

	glm::mat4 perspectiveProjMatrix;
	glm::mat4 orthographicProjMatrix;

	float movementSpeed = 10.f;	
	float mouseSensitivity = 0.005f;
};