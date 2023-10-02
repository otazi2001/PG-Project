#pragma once
#include "Common.h"

class Window {
public:
	~Window();

	bool Init();
	bool Deinit();

	void SwapBuffers();

	int Width() { return m_width; }
	int Height() { return m_height; }

	GLFWwindow* getWindow() const { return m_window; }

private:
	GLFWwindow* m_window;
	const char* m_name;

	int m_width;
	int m_height;


};