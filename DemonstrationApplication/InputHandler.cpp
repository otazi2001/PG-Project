#include "InputHandler.h"

void InputHandler::Initialize(GLFWwindow* window) {
    _window = window;
    _keyCallbacks = map<int, KeyboardCallback>();

    pollMousePosition();

    _lastMousePosX = _currMousePosX;
    _lastMousePosY = _currMousePosY;
}

void InputHandler::Update(double elapsedTime) {
    // handle keyboard inputs
    for (const auto& entry : _keyCallbacks) {
        int key = entry.first;
        KeyboardCallback callback = entry.second;

        if (isKeyPressed(key)) {
            callback(KeyboardEvent(elapsedTime, key));
        }
    }

    // handle mouse position changes
    _lastMousePosX = _currMousePosX;
    _lastMousePosY = _currMousePosY;

    pollMousePosition();

    double delta_x = _currMousePosX - _lastMousePosX;
    double delta_y = _currMousePosY - _lastMousePosY;

    if (delta_x != 0.0 || delta_y != 0.0) {
        _mouseCallback(MouseEvent(elapsedTime, delta_x, delta_y));
    }
}

void InputHandler::AddMouseCallback(MouseCallback callback) {
    _mouseCallback = callback;
}

void InputHandler::AddKeyboardCallback(int key, KeyboardCallback callback) {
    _keyCallbacks.insert(make_pair(key, callback));
}

bool InputHandler::isKeyPressed(int key) {
    return glfwGetKey(_window, key) == GLFW_PRESS;
}

void InputHandler::pollMousePosition() {
    glfwGetCursorPos(_window, &_currMousePosX, &_currMousePosY);
}