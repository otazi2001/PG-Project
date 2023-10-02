#pragma once
#include "Common.h"

#include "Window.h"
#include "InputHandler.h"
#include "Shader.h"
#include "Camera.h"
#include "Capture.h"
#include "Model.h"

#include "GameObject.h"
#include "Quad.cpp"
#include "Cube.cpp"

class Pipeline {
public:

	Pipeline();
	~Pipeline();

	bool Initialize();
	void Execute();

	void Update();
	void Render();

	void InitializeInputs();
	void UpdateLandmarks(std::vector<glm::vec3> positions);
	void RenderLine(const glm::vec3& start, const glm::vec3& end);

	Camera* GetCamera();

private:

	Window m_window;

	InputHandler* m_inputHandler;
	Capture* m_capture;
	Shader* m_shader;
	Camera* m_camera;

	GameObject* m_CaptureDisplay;
	GameObject* m_Floor;


	Model	m_model;
	std::vector<GameObject*> m_landmarks;
	std::vector<glm::vec3> connector_verts;

	double m_deltaTime;
	double m_totalTime = 0;
	int m_loop_itterations = 0;
};