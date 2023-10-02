#include <Kinect.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>
#include <chrono>
#include <Windows.Storage.Streams.h>

struct FrameData {
    RGBQUAD* bColor;
    UINT16* bDepth;
    int colorWidth;
    int colorHeight;
    int depthWidth;
    int depthHeight;
    float time;

};

std::queue<FrameData> frameBufferQueue;  // Queue to hold captured color and depth buffers
std::mutex frameMutex;                                   // Mutex for synchronizing access to the queue

// Create a directory to save images and depth data
const std::string imageDirectory = "C:/Users/Othmane Tazi/Desktop/KinectOutput/Images/";
const std::string depthDirectory = "C:/Users/Othmane Tazi/Desktop/KinectOutput/Depth/";

const std::string csvFileName = "C:/Users/Othmane Tazi/Desktop/KinectOutput/FrameData.csv";
std::ofstream csvFile(csvFileName);

const int Totalframes = 100;
const int startframe = 0;



IKinectSensor* pKinectSensor = nullptr;
IMultiSourceFrameReader* pMultiSourceFrameReader = nullptr;

void captureFrameBuffers(IMultiSourceFrameReader* pMultiSourceFrameReader) {
    int frameNumber = Totalframes - startframe;
    auto start_time = std::chrono::high_resolution_clock::now();


    while (frameNumber > 0) {

        // set frame rate
        Sleep(1000 / 30);
        // Acquire a multi-source frame
        IMultiSourceFrame* pMultiSourceFrame = nullptr;
        HRESULT hr = pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame);

        if (SUCCEEDED(hr) && pMultiSourceFrame != nullptr) {
            IColorFrameReference* pColorFrameReference = nullptr;
            IDepthFrameReference* pDepthFrameReference = nullptr;

            // Get color frame reference
            hr = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);

            // Get depth frame reference
            hr = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);

            // Acquire color frame
            IColorFrame* pColorFrame = nullptr;
            hr = pColorFrameReference->AcquireFrame(&pColorFrame);

            // Acquire depth frame
            IDepthFrame* pDepthFrame = nullptr;
            hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);

            if (SUCCEEDED(hr) && pColorFrame != nullptr && pDepthFrame != nullptr) {
                FrameData frame;

                // Extract frame data (similar to your code)
                IFrameDescription* pColorFrameDesc = nullptr;
                IFrameDescription* pDepthFrameDesc = nullptr;

                hr = pColorFrame->get_FrameDescription(&pColorFrameDesc);
                hr = pDepthFrame->get_FrameDescription(&pDepthFrameDesc);

                pColorFrameDesc->get_Width(&frame.colorWidth);
                pColorFrameDesc->get_Height(&frame.colorHeight);
                pColorFrameDesc->Release();

                pDepthFrameDesc->get_Width(&frame.depthWidth);
                pDepthFrameDesc->get_Height(&frame.depthHeight);
                pDepthFrameDesc->Release();

                UINT bufferSizeC = 0;
                UINT bufferSizeD = 0;
                RGBQUAD* pntrC = nullptr;
                UINT16* pntrD = nullptr;


                // Get Data buffers from color and depth frames
                pColorFrame->AccessRawUnderlyingBuffer(&bufferSizeC, reinterpret_cast<BYTE**>(&pntrC));
                pDepthFrame->AccessUnderlyingBuffer(&bufferSizeD, &pntrD);
                bufferSizeD = bufferSizeD * 2; ////////////////////// ITS SO BROKENNNN!!! USE A DIFFERENT SENSOR

                frame.bColor = new RGBQUAD[bufferSizeC];
                std::memcpy(frame.bColor, pntrC, bufferSizeC);

                frame.bDepth = new UINT16[bufferSizeD];
                std::memcpy(frame.bDepth, pntrD, bufferSizeD);

                auto current_time = std::chrono::high_resolution_clock::now();
                auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);

                frame.time = elapsed_time.count() / 1000.0f;
                start_time = current_time;

                // Push the frame into the queue
                try {
                    std::lock_guard<std::mutex> lock(frameMutex);
                    frameBufferQueue.push(frame);
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception when pushing to queue: " << e.what() << std::endl;
                }

                frameNumber--;

                int progress = ((Totalframes - frameNumber) * 50) / Totalframes;
                std::cout << "Capturing Progress :";
                std::cout << "[";

                for (int i = 0; i < progress; i++) {
                    std::cout << "|";
                }

                for (int i = 0; i < 50 - progress; i++) {
                    std::cout << " ";
                }

                std::cout << "] " << std::fixed << std::setprecision(1) << frameNumber << "\r";
            }

            // Release the acquired frames and references
            if (pColorFrame != nullptr) {
                pColorFrame->Release();
            }
            if (pDepthFrame != nullptr) {
                pDepthFrame->Release();
            }
            if (pColorFrameReference != nullptr) {
                pColorFrameReference->Release();
            }
            if (pDepthFrameReference != nullptr) {
                pDepthFrameReference->Release();
            }
        }
    }
}

void processBuffers() {
    int frameNumber = Totalframes - startframe;

    while (frameNumber > 0) {
        // Lock the mutex before accessing the queue
        std::lock_guard<std::mutex> lock(frameMutex);

        // Check if the queue is empty
        if (!frameBufferQueue.empty()) {

            if (frameNumber == Totalframes - startframe) {
                frameNumber--;
                continue;
            }

            FrameData frame = frameBufferQueue.front();
            frameBufferQueue.pop();

            cv::Mat ImageRGB;

            if (frame.bColor != nullptr && frame.bDepth != nullptr) {
                cv::Mat colorImage(frame.colorHeight, frame.colorWidth, CV_8UC2, reinterpret_cast<uchar*>(frame.bColor));
                cv::cvtColor(colorImage, ImageRGB, cv::COLOR_YUV2BGR_YUY2);
                std::string imageName = imageDirectory + std::to_string(frameNumber + startframe) + ".png";
                cv::imwrite(imageName, ImageRGB);

                BYTE* dest; // Declare the pointer
                const UINT16* curr = (const UINT16*)frame.bDepth;
                const UINT16* dataEnd = curr + (frame.depthWidth * frame.depthHeight);

                // Calculate the size needed for the buffer (assuming 4 channels for BGRX)
                size_t bufferSize = frame.depthWidth * frame.depthHeight * 4;
                
                
                // Allocate memory for the buffer
                dest = new BYTE[bufferSize];
                BYTE* destStart = dest; // Store the start of the buffer

                while (curr < dataEnd) {
                    UINT16 depth = (*curr++);
                    for (int i = 0; i < 3; ++i)
                        *dest++ = (BYTE)(depth % 256);
                    *dest++ = 0xff;
                }
                cv::Mat depthImage(frame.depthHeight, frame.depthWidth, CV_8UC4);

                

                // Copy the data from 'dest' to the Mat
                memcpy(depthImage.data, destStart, frame.depthHeight * frame.depthWidth * 4);

                // Save the Mat as a PNG image (assuming 'depthImage' is your cv::Mat containing the depth image)
                std::string depthName = depthDirectory + std::to_string(frameNumber + startframe) + ".png";

                if (!depthImage.empty()) {
                    // Save the Mat as a PNG image
                    bool saved = cv::imwrite(depthName, depthImage);

                    if (saved) {
                        std::cout << "Image saved successfully." << std::endl;
                    }
                    else {
                        std::cerr << "Failed to save the image." << std::endl;
                    }
                }
                else {
                    std::cerr << "DepthImage Mat is empty or uninitialized." << std::endl;
                }

                // Save the depth image as a PNG file
                cv::imwrite("depth_image.png", depthImage);

                if (csvFile.is_open()) {
                    csvFile << frameNumber + startframe << ", " << frame.time << "\n";
                }

                frameNumber--;
            }
        }
    }
}

int main() {
    /////////////////////////////////////
    //           Kinect Setup          //
    /////////////////////////////////////
    std::cout << " Intitializing Kinect Secsor..." << std::endl;


    if (FAILED(GetDefaultKinectSensor(&pKinectSensor))) {
        return 0;
    }

    if (pKinectSensor) {
        pKinectSensor->Open();

        // Access the color camera intrinsics
        IColorFrameSource* colorFrameSource = nullptr;
        pKinectSensor->get_ColorFrameSource(&colorFrameSource);
        pKinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Color | FrameSourceTypes::FrameSourceTypes_Depth, &pMultiSourceFrameReader);

    }
    else {
        return 0;
    }

    std::cout << "Kinect Sensor Initialialized" << std::endl;

    // Open the CSV file
    if (csvFile.is_open()) {
        csvFile << "Frame, time\n";
    }

    /////////////////////////////////////
    //          processes Setup        //
    /////////////////////////////////////

    std::cout << " Starting Color and Depth Frames Capture..." << std::endl;
    captureFrameBuffers(pMultiSourceFrameReader);
    std::cout << "Frames Capture Completed!" << std::endl;

    std::cout << " Starting Processing and Saving of Captured Frames..." << std::endl;
    processBuffers();
    std::cout << "Processing and Saving Completed!" << std::endl;

    return 0;

}


