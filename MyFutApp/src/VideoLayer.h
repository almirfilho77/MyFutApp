#ifndef __VIDEO_LAYER_H__
#define __VIDEO_LAYER_H__
#pragma once

#include "CatolYeah.h"
#include "opencv2/opencv.hpp"
#include "ObjectDetector.h"

class VideoLayer : public CatolYeah::Layer
{
public:
	VideoLayer(const std::string& videoPath);
	virtual ~VideoLayer();

	virtual void OnAttach() override;
	virtual void OnUpdate(CatolYeah::Timestep ts) override;
	virtual void OnEvent(CatolYeah::Event& e) override;

private:
	std::string m_videoPath;
	float m_frameRate;
	CatolYeah::Ref<CatolYeah::Texture2D> m_currentFrame;
	cv::VideoCapture m_videoCapture;
	CatolYeah::Ref<ObjectDetector> m_ObjectDetector;

	glm::vec3 m_squarePosition = { 0.0f, 0.0f, 0.0f };

private:
	bool m_MouseEventHandler(CatolYeah::MouseButtonPressedEvent& e);
};

#endif //__VIDEO_LAYER_H__
