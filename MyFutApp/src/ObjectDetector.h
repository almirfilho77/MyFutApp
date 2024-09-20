#ifndef __OBJECT_DETECTOR_H__
#define __OBJECT_DETECTOR_H__
#pragma once

#include "inference.h"

class ObjectDetector
{
public:
	ObjectDetector(const std::string &assetsPath, bool useCuda = false);
	~ObjectDetector();

	void Detect(cv::Mat img);

private:
	int m_ReadCocoYaml(YOLO_V8*& p);
	YOLO_V8* m_Detector = nullptr;
	DL_INIT_PARAM m_Params;
	std::string m_AssetsPath;
};

#endif //__OBJECT_DETECTOR_H__