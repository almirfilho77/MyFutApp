#include "ObjectDetector.h"

#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <random>

#include "CatolYeah/Core/Logger.h"

ObjectDetector::ObjectDetector(const std::string& assetsPath, bool useCuda)
{
    m_Detector = new YOLO_V8;
    m_AssetsPath = assetsPath;
    m_ReadCocoYaml(m_Detector);
    m_Params.rectConfidenceThreshold = 0.4;
    m_Params.iouThreshold = 0.5;
    m_Params.imgSize = { 480, 640 };
    std::filesystem::path yoloModelPath(m_AssetsPath);
    if (useCuda)
    {
        CY_INFO("DetectTest -- Cuda is enabled... Using GPU!");
        yoloModelPath = yoloModelPath / "yolov8m_fp16.onnx";
        m_Params.modelPath = yoloModelPath.string();
        m_Params.cudaEnable = true;

        // GPU FP32 inference
        //m_Params.modelType = YOLO_DETECT_V8;
        // GPU FP16 inference
        m_Params.modelType = YOLO_DETECT_V8_HALF;
        //Note: change fp16 onnx model
    }
    else
    {
        CY_INFO("DetectTest -- Cuda is disabled... Using CPU!");
        yoloModelPath = yoloModelPath / "yolov8m.onnx";
        m_Params.modelPath = yoloModelPath.string();
        // CPU inference
        m_Params.modelType = YOLO_DETECT_V8;
        m_Params.cudaEnable = false;
    }
    m_Detector->CreateSession(m_Params);
}

ObjectDetector::~ObjectDetector()
{
}

void ObjectDetector::Detect(cv::Mat img)
{
    if (m_Detector == nullptr)
    {
        CY_ERROR("Unitialized object detector");
        return;
    }
    std::vector<DL_RESULT> res;
    m_Detector->RunSession(img, res);

    for (auto& re : res)
    {
        cv::RNG rng(cv::getTickCount());
        cv::Scalar color(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

        cv::rectangle(img, re.box, color, 3);

        float confidence = floor(100 * re.confidence) / 100;
        //std::cout << std::fixed << std::setprecision(2);
        std::string label = m_Detector->classes[re.classId] + " " +
            std::to_string(confidence).substr(0, std::to_string(confidence).size() - 4);

        cv::rectangle(
            img,
            cv::Point(re.box.x, re.box.y - 25),
            cv::Point(re.box.x + label.length() * 15, re.box.y),
            color,
            cv::FILLED
        );

        cv::putText(
            img,
            label,
            cv::Point(re.box.x, re.box.y - 5),
            cv::FONT_HERSHEY_SIMPLEX,
            0.75,
            cv::Scalar(0, 0, 0),
            2
        );


    }
    //std::cout << "Press any key to exit" << std::endl;
    //cv::imshow("Result of Detection", img);
    //cv::waitKey(0);
    //cv::destroyAllWindows();
}

int ObjectDetector::m_ReadCocoYaml(YOLO_V8*& p) {
    // Open the YAML file
    std::filesystem::path cocoYamlPath(m_AssetsPath);
    cocoYamlPath = cocoYamlPath / "coco.yaml";
    std::ifstream file(cocoYamlPath.string());
    if (!file.is_open())
    {
        CY_ERROR("Failed to open file {0}", cocoYamlPath.string());
        return 1;
    }

    // Read the file line by line
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    // Find the start and end of the names section
    std::size_t start = 0;
    std::size_t end = 0;
    for (std::size_t i = 0; i < lines.size(); i++)
    {
        if (lines[i].find("names:") != std::string::npos)
        {
            start = i + 1;
        }
        else if (start > 0 && lines[i].find(':') == std::string::npos)
        {
            end = i;
            break;
        }
    }

    // Extract the names
    std::vector<std::string> names;
    for (std::size_t i = start; i < end; i++)
    {
        std::stringstream ss(lines[i]);
        std::string name;
        std::getline(ss, name, ':'); // Extract the number before the delimiter
        std::getline(ss, name); // Extract the string after the delimiter
        names.push_back(name);
    }

    m_Detector->classes = names;
    return 0;
}