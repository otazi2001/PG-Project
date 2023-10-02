#include "Capture.h"

bool Capture::Initialize() {
    //initialize capture device
    m_capture = cv::VideoCapture(0);

    if (!m_capture.isOpened()) {
        cout << "Error opening the capture device!" << endl;
        return false;
    }

    // Get frame dimensions
    frame_width = static_cast<int>(m_capture.get(cv::CAP_PROP_FRAME_WIDTH));
    frame_height = static_cast<int>(m_capture.get(cv::CAP_PROP_FRAME_HEIGHT));

    int smallest_axis_size = std::min(frame_width, frame_height);

    x_min = (frame_width / 2) - (smallest_axis_size / 2) + (smallest_axis_size * 0.2f);
    x_max = (frame_width / 2) + (smallest_axis_size / 2) - (smallest_axis_size * 0.2f);
    y_min = (frame_height / 2) - (smallest_axis_size / 2) + (smallest_axis_size * 0.2f);
    y_max = (frame_height / 2) + (smallest_axis_size / 2) - (smallest_axis_size * 0.2f);




    // Initialize frame 
    glGenTextures(1, &m_frameTexID);
    
    // Bind texture
    glBindTexture(GL_TEXTURE_2D, m_frameTexID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Specify the texture format and allocate memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_capture.get(cv::CAP_PROP_FRAME_WIDTH), m_capture.get(cv::CAP_PROP_FRAME_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Check for OpenGL Errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
        return false;
    }

    return true;
}

void Capture::Update() {
    // Read New Frame
    m_capture.read(m_frame);

    cv::Mat copy = m_frame.clone();

    cv::Point pt1(x_min, y_min);
    cv::Point pt2(x_min, y_max);
    cv::Point pt3(x_max, y_max);
    cv::Point pt4(x_max, y_min);

    // Draw the lines of the rectangle
    cv::line(copy, pt1, pt2, cv::Scalar(0, 0, 255), 5);
    cv::line(copy, pt2, pt3, cv::Scalar(0, 0, 255), 5);
    cv::line(copy, pt3, pt4, cv::Scalar(0, 0, 255), 5);
    cv::line(copy, pt4, pt1, cv::Scalar(0, 0, 255), 5);

    // Bind Texture
    glBindTexture(GL_TEXTURE_2D, m_frameTexID);

    // Update Texture
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, copy.cols, copy.rows, GL_BGR, GL_UNSIGNED_BYTE, copy.data);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Capture::Bind() {
    glBindTexture(GL_TEXTURE_2D, m_frameTexID);
}

GLuint Capture::getTextureID() {
    return m_frameTexID;
}

cv::Mat Capture::GetMOCRNNInput() {

    // Apply crop
    cv::Rect roi(x_min, y_min, x_max - x_min, y_max - y_min);
    m_frame = m_frame(roi);

    // Apply Gaussian blur
    cv::Size kernelSize(5, 5);
    cv::GaussianBlur(m_frame, m_frame, kernelSize, 0);

    // Convert to grayscale
    cv::cvtColor(m_frame, m_frame, cv::COLOR_RGB2GRAY);

    // Resize image to 256x256
    cv::Mat resized_image;
    cv::resize(m_frame, resized_image, cv::Size(256, 256));

    // Apply Sobel edge detection in the horizontal direction
    cv::Mat sobel_x;
    cv::Sobel(resized_image, sobel_x, CV_64F, 1, 0, 3);

    // Apply Sobel edge detection in the vertical direction
    cv::Mat sobel_y;
    cv::Sobel(resized_image, sobel_y, CV_64F, 0, 1, 3);

    // Calculate the absolute difference
    cv::Mat abs_sobel_x;
    cv::absdiff(sobel_x, cv::Scalar(0), abs_sobel_x);
    cv::Mat abs_sobel_y;
    cv::absdiff(sobel_y, cv::Scalar(0), abs_sobel_y);

    // Combine horizontal and vertical edge images
    cv::Mat hand_edges = cv::Mat::zeros(resized_image.rows, resized_image.cols, CV_64F);
    cv::addWeighted(abs_sobel_x, 0.5, abs_sobel_y, 0.5, 0, hand_edges);

    // Apply thresholding
    cv::Mat hand_thrshld;
    cv::threshold(hand_edges, hand_thrshld, 31, 255, cv::THRESH_BINARY);

    

    return hand_thrshld;
}