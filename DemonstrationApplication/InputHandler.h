#pragma once
#include "Common.h"
#include "Typedefs.h"

class InputHandler {
public:
	void Initialize(GLFWwindow* window);

	void Update(double elapsedTime);

	void AddMouseCallback(MouseCallback callback);
	void AddKeyboardCallback(int key, KeyboardCallback callback);

	bool isKeyPressed(int key);
	void pollMousePosition();

private:

	GLFWwindow* _window;

	map<int, KeyboardCallback> _keyCallbacks;

	MouseCallback _mouseCallback;
	double _currMousePosX;
	double _currMousePosY;
	double _lastMousePosX;
	double _lastMousePosY;

};