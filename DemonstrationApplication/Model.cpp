#include "Model.h"

bool Model::Initialize(string path_pth) {
    // load in the model
    try {
        model = torch::jit::load(path_pth.c_str());
    }
    catch (const c10::Error& e) {
        std::cerr << "Error loading the model: " << path_pth << ' ' << e.msg() << "\n";
        return false;
    }

    return true;
}


std::vector<glm::vec3> Model::GetLandmarksPositions(cv::Mat input_image) {
    // the image already comes in the correct size
    input_image.convertTo(input_image, CV_32FC1); // Convert to float32
    input_image /= 255.0; // Normalize to [0, 1] range

    // Convert the OpenCV Mat to a torch::Tensor
    torch::Tensor input_tensor = torch::from_blob(input_image.data, { 1, 1, 256, 256 }, torch::kFloat);

    // prepares torch tensor for the MOCRNN
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(input_tensor);
    std::cout << inputs.size() << "\n";
    // Run the MOCRNN and get the predictions
    torch::Tensor output;
    try {
        output = model.forward(inputs ).toTensor();
    }
    catch (const c10::Error& e) {
        std::cerr << "Error during forward pass: " << e.msg() << std::endl;
    }


    // format the outputs into glm::vec3 and add them to a vector
    std::vector<glm::vec3> landmarks_positions;
    for (int i = 0; i < output.size(1); i += 3) {
        float x = output[0][i].item<float>();
        float y = output[0][i + 2].item<float>();
        float z = output[0][i + 1].item<float>();

        glm::vec3 landmark(x, y, z);
        landmarks_positions.push_back(landmark);
    }

    //std::cout << landmarks_positions.size() << "\n";

    return landmarks_positions;
}