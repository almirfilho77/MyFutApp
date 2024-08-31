#ifndef __DRAWING_LAYER_H__
#define __DRAWING_LAYER_H__
#pragma once

#include "CatolYeah.h"

class DrawingLayer : public CatolYeah::Layer
{
public:
	uint32_t width, height;
public:
	DrawingLayer(const std::string& pitchImagePath);
	virtual void OnAttach() override;
	virtual void OnEvent(CatolYeah::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	std::string m_pitchImagePath;
	CatolYeah::Ref<CatolYeah::Texture2D> m_pitchImage;
private:
	bool m_OnWindowResize(CatolYeah::WindowResizeEvent& windowResizeEvent);

};

#endif /*__DRAWING_LAYER_H__*/
