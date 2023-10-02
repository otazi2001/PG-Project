#include "Window.h"
#include "Common.h"

Window::~Window() { 
	glfwTerminate(); 
}

bool Window::Init() {
    m_width = 1600;
    m_height = 900;
    m_name = "Gesture Pipeline";

    if (!glfwInit()) {
        // Error handling for GLFW initialization failure
        return false;
    }

    // Set OpenGL version and profile (we are using core profile here)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    // Create the window
    m_window = glfwCreateWindow(m_width, m_height, m_name, nullptr, nullptr);
    if (!m_window) {
        // Error handling for window creation failure
        glfwTerminate();
        return false;
    }

    // Make the window's OpenGL context current
    glfwMakeContextCurrent(m_window);

    // Initialize GLEW after the OpenGL context is set
    if (glewInit() != GLEW_OK) {
        // Error handling for GLEW initialization failure
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    // Set the viewport size based on the window dimensions
    glViewport(0, 0, m_width, m_height);

    return true;
}

bool Window::Deinit() {
    return glfwWindowShouldClose(m_window);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_window);
}