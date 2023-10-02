#pragma once
#include "Common.h"

class Capture {
public:
	bool Initialize();
	void Update();
	void Bind();
	GLuint getTextureID();

	cv::Mat GetMOCRNNInput();

	int GetWidth() { return frame_width; }
	int GetHeight() { return frame_height; }

private:
	cv::VideoCapture m_capture;
	cv::Mat m_frame;
	GLuint m_frameTexID;
	int frame_width;
	int frame_height;

	int x_max;
	int x_min;
	int y_max;
	int y_min;
};