#include "CatolYeah.h"
#include "CatolYeah/Core/EntryPoint.h"

#include <filesystem>
#include <iostream>

#include "imgui.h"
#include "DrawingLayer.h"
#include "VideoLayer.h"
#include "DebugObject.h"

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

namespace fs = std::filesystem;

class DebugLayer : public CatolYeah::Layer
{
private:
	std::shared_ptr<DebugObject> m_debugObject;
public:
	DebugLayer(std::shared_ptr<DebugObject> debugObject) 
		: m_debugObject(debugObject)
	{
	}

	void OnImGuiRender() override
	{
		static bool show = true;

		//CatolYeah::RenderCommand::Clear();

		ImGui::ShowDemoWindow(&show);

		ImGui::Begin("GPU Info", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::End();

		if (ImGui::Begin("Debug handles", 0, 0))
		{
			ImGui::Checkbox("DrawCrossHair", &m_debugObject->DrawCrossHair);
			ImGui::Checkbox("Little pitch corner points", &m_debugObject->LittlePitchPoints);
		}
		ImGui::End();
	}
};

class App : public CatolYeah::Application
{
public:
	std::shared_ptr<DebugObject> debugObject;

public:
	App(const CatolYeah::ApplicationSpecs &appSpecs)
		:	CatolYeah::Application(appSpecs), debugObject(std::make_shared<DebugObject>())
	{
		CY_DEBUG("Init App class");
		CY_DEBUG("Assets path: {0}", appSpecs.AssetsPath);
		CatolYeah::RenderCommand::SetViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());

		fs::path pitchTexturePath = fs::current_path() / "assets" / "textures" / "football_pitch_2D_base2.png";
		//fs::path texturePath = fs::current_path() / "assets" / "textures" / "football_1.png";
		//fs::path videoPath = fs::current_path() / "assets" / "football-videos" / "video.mp4";
		fs::path videoPath = fs::current_path() / "assets" / "football-videos" / "A1606b0e6_0 (9).mp4";
		CY_DEBUG("Video path = {0}", videoPath.string());
		PushLayer(new VideoLayer(videoPath.string()));

		PushOverlay(new DrawingLayer(pitchTexturePath.string(), debugObject));
		PushOverlay(new DebugLayer(debugObject));
	}
};

CatolYeah::Application* CatolYeah::CreateApplication()
{
	fs::path assetsPath = fs::current_path() / "assets" / "shaders";
	CatolYeah::ApplicationSpecs appSpecs{ "MyFutApp", 1280, 720, assetsPath.string(), true };
	return new App(appSpecs);
}