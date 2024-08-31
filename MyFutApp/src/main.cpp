#include "CatolYeah.h"
#include "CatolYeah/Core/EntryPoint.h"

#include <filesystem>
#include <iostream>

#include "imgui.h"
#include "DrawingLayer.h"

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

class BackgroundLayer : public CatolYeah::Layer
{
public:
	BackgroundLayer(const std::string& bgImagePath)
		: Layer("BackgroundLayer"), m_backgroundImagePath(bgImagePath)
	{
		CY_ASSERT(!bgImagePath.empty(), "Empty background image!!");
		m_squarePosition = { 0.0f, 0.0f, 0.0f };
	}

	void OnAttach() override
	{
		m_backgroundImage = CatolYeah::Texture2D::Create(m_backgroundImagePath);
	}
	
	void OnUpdate(CatolYeah::Timestep ts)
	{

		// Square translation
		if (CatolYeah::Input::IsKeyPressed(CY_KEY_L))
			m_squarePosition.x += 5.0f * ts;
		else if (CatolYeah::Input::IsKeyPressed(CY_KEY_J))
			m_squarePosition.x -= 5.0f * ts;

		if (CatolYeah::Input::IsKeyPressed(CY_KEY_I))
			m_squarePosition.y += 5.0f * ts;
		else if (CatolYeah::Input::IsKeyPressed(CY_KEY_K))
			m_squarePosition.y -= 5.0f * ts;

		// Render commands
		CatolYeah::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		CatolYeah::RenderCommand::Clear();

		CatolYeah::Renderer2D::BeginScene();

		//CY_INFO("Square position [x:{0}] [y:{1}", m_squarePosition.x, m_squarePosition.y);
		CatolYeah::Renderer2D::DrawQuad({ m_squarePosition.x, m_squarePosition.y, 0.2f }, { 1.0f, 1.0f }, m_backgroundImage);				// Texture closes to the screen than flat color quad
		CatolYeah::Renderer2D::EndScene();
	}

	void OnEvent(CatolYeah::Event& e)
	{
		CatolYeah::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<CatolYeah::MouseButtonPressedEvent>(CY_BIND_EVENT_FN(BackgroundLayer::m_MouseEventHandler));
	}

private:
	bool m_MouseEventHandler(CatolYeah::MouseButtonPressedEvent& e)
	{
		CY_DEBUG("Mouse click pos: {0}x{1}", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
		return EVENT_RETURN_PASS_ON;
	}

private:	
	std::string m_backgroundImagePath;
	CatolYeah::Ref<CatolYeah::Texture2D> m_backgroundImage;

	glm::vec3 m_squarePosition = { 0.0f, 0.0f, 0.0f };
};

class App : public CatolYeah::Application
{
public:
	App(const std::string& windowTitle = "MyApp",
		uint32_t windowWidth = 1920,
		uint32_t windowHeight = 1080,
		std::string_view assetsPath = "",
		bool setVSync = true)
		:	CatolYeah::Application(windowTitle, windowWidth, windowHeight, assetsPath)
	{
		CY_DEBUG("Init App class");
		CY_DEBUG("Assets path: {0}", assetsPath);
		CatolYeah::RenderCommand::SetViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());

		fs::path pitchTexturePath = fs::current_path() / "assets" / "textures" / "football_pitch_2D_base2.png";
		fs::path texturePath = fs::current_path() / "assets" / "textures" / "football_1.png";
		//fs::path texturePath = fs::current_path() / "assets" / "textures" / "ronaldinho.png";
		PushLayer(new BackgroundLayer(texturePath.string()));

		PushOverlay(new DrawingLayer(pitchTexturePath.string()));
		PushOverlay(new DebugLayer());
	}
};

CatolYeah::Application* CatolYeah::CreateApplication()
{
	fs::path assetsPath = fs::current_path() / "assets" / "shaders";
	return new App("MyFutApp", 1280, 720, assetsPath.string());
}