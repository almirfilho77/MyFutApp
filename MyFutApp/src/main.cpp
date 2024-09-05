#include "CatolYeah.h"
#include "CatolYeah/Core/EntryPoint.h"

#include <filesystem>
#include <iostream>

#include "imgui.h"
#include "DrawingLayer.h"
#include "VideoLayer.h"

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

namespace fs = std::filesystem;

#define OPEN_CANVAS_BUTTON_LABEL	"Open Canvas"
#define CLOSE_CANVAS_BUTTON_LABEL	"Close Canvas"

class DebugLayer : public CatolYeah::Layer
{
public:
	DebugLayer() {}

	void OnImGuiRender() override
	{
		static bool show = true;

		//CatolYeah::RenderCommand::Clear();

		ImGui::ShowDemoWindow(&show);

		ImGui::Begin("GPU Info", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::End();
	}
};

class App : public CatolYeah::Application
{
public:
	App(const CatolYeah::ApplicationSpecs &appSpecs)
		:	CatolYeah::Application(appSpecs)
	{
		CY_DEBUG("Init App class");
		CY_DEBUG("Assets path: {0}", appSpecs.AssetsPath);
		CatolYeah::RenderCommand::SetViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());

		fs::path pitchTexturePath = fs::current_path() / "assets" / "textures" / "football_pitch_2D_base2.png";
		//fs::path texturePath = fs::current_path() / "assets" / "textures" / "football_1.png";
		fs::path videoPath = fs::current_path() / "assets" / "football-videos" / "video.mp4";
		CY_DEBUG("Video path = {0}", videoPath.string());
		PushLayer(new VideoLayer(videoPath.string()));

		PushOverlay(new DrawingLayer(pitchTexturePath.string()));
		PushOverlay(new DebugLayer());
	}
};

CatolYeah::Application* CatolYeah::CreateApplication()
{
	fs::path assetsPath = fs::current_path() / "assets" / "shaders";
	CatolYeah::ApplicationSpecs appSpecs{ "MyFutApp", 1280, 720, assetsPath.string(), true };
	return new App(appSpecs);
}