#pragma once
#include "Common.h"
#include "torch/torch/script.h"

class Model {
public:
	bool Initialize(string path_onxx);

	std::vector<glm::vec3> GetLandmarksPositions(cv::Mat input_image);

private:
	torch::jit::script::Module model;

};