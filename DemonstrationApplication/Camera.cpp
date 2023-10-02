#include "Camera.h"
#include "Common.h"

void Camera::Initialize(int width, int height) {
	// setting up camera vectors
	position = glm::vec3(0.f);
	upVector = glm::vec3(0.f, 1.f, 0.f);
	frontVector = glm::vec3(0.f, 0.f, 1.f);
	strafeVector = glm::cross(frontVector, upVector);

	// defining projection matricies
	perspectiveProjMatrix = glm::perspective(glm::radians(100.f), float(width / height), 0.01f, 100.f);
	orthographicProjMatrix = glm::ortho(0.f, float(width), 0.f, float(height));
}

glm::mat4 Camera::GetPerspectiveProjMatrix() {
	return perspectiveProjMatrix;
}

glm::mat4 Camera::GetOrthographicProjMatrix() {
	return orthographicProjMatrix;
}

glm::mat4 Camera::GetPerspectiveViewMatrix() {
	return glm::lookAt(position, position+frontVector, upVector);
}
glm::mat4 Camera::GetOrthographicViewMatrix() {
	return glm::lookAt(glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f));
}

glm::mat3 Camera::GetNormalMatrix(glm::mat4 ViewMatrix) {
	return glm::transpose(glm::inverse(glm::mat3(ViewMatrix)));
}


void Camera::Forwards(KeyboardEvent e) {
	position = position + (glm::normalize(frontVector) * float(e.elapsedTime) * movementSpeed);
}

void Camera::Backwards(KeyboardEvent e) {
	position = position - (glm::normalize(frontVector) * float(e.elapsedTime) * movementSpeed);
}

void Camera::StrafeLeft(KeyboardEvent e) {
	position = position - (glm::normalize(strafeVector) * float(e.elapsedTime) * movementSpeed);
}

void Camera::StrafeRight(KeyboardEvent e) {
	position = position + (glm::normalize(strafeVector) * float(e.elapsedTime) * movementSpeed);
}

void Camera::SetViewByMouse(MouseEvent e) {

	frontVector = glm::normalize(frontVector + (e.deltaYaw * mouseSensitivity) * strafeVector - (e.deltaPitch * mouseSensitivity) * upVector);
	glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(frontVector, globalUp));
	upVector = glm::cross(right, frontVector);
	strafeVector = glm::cross(frontVector, upVector);
}