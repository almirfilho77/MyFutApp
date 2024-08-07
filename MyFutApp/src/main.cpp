#include "CatolYeah.h"
#include "CatolYeah/Core/EntryPoint.h"
#include <filesystem>
#include <iostream>
#include "imgui.h"

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

class DrawingLayer : public CatolYeah::Layer
{
public:
	uint32_t width, height;

private:
	bool m_OnWindowResize(CatolYeah::WindowResizeEvent& windowResizeEvent)
	{
		width = windowResizeEvent.GetWindowWidth();
		height = windowResizeEvent.GetWindowHeight();
		return EVENT_RETURN_PASS_ON;
	}

public:
	DrawingLayer(const std::string &pitchImagePath)
		:	m_pitchImagePath(pitchImagePath)
	{
		width = CatolYeah::Application::Get().GetWindow().GetWidth();
		height = CatolYeah::Application::Get().GetWindow().GetHeight();
	}

	void OnAttach() override
	{
		m_pitchImage = CatolYeah::Texture2D::Create(m_pitchImagePath);
	}

	virtual void OnEvent(CatolYeah::Event& event) override
	{
		CatolYeah::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<CatolYeah::WindowResizeEvent>(CY_BIND_EVENT_FN(DrawingLayer::m_OnWindowResize));
	}

	void OnImGuiRender() override
	{
		static bool p_open = true;

		//CatolYeah::RenderCommand::Clear();

		/*ImGui::Begin("Open/Close Canvas", 0, ImGuiWindowFlags_NoDecoration);
		static std::string buttonLabel = CLOSE_CANVAS_BUTTON_LABEL;
		if (ImGui::Button(buttonLabel.c_str()))
		{
			p_open = !p_open;
			if (p_open) buttonLabel = CLOSE_CANVAS_BUTTON_LABEL;
			else		buttonLabel = OPEN_CANVAS_BUTTON_LABEL;
		}
		ImGui::End();*/

		auto winPos = ImGui::GetWindowPos();
		//CY_DEBUG("Window size: {0}x{1}", width, height);
		//CY_DEBUG("Window pos: {0}x{1}", winPos.x, winPos.y);

		auto imageWidth = static_cast<uint32_t>(m_pitchImage->GetWidth() * 0.4f);
		auto imageHeight = static_cast<uint32_t>(m_pitchImage->GetHeight() * 0.4f);
		//CY_DEBUG("Image size: {0}x{1}", imageWidth, imageHeight);

		ImVec2 pmin = ImVec2(static_cast<uint32_t>(0.5f * (width - imageWidth)) + winPos.x, height - imageHeight - 100 + winPos.y);
		ImVec2 pmax = ImVec2(pmin.x + imageWidth, pmin.y + imageHeight);
		//CY_DEBUG("pmin: {0}x{1} / pmax: {2}x{3}", pmin.x, pmin.y, pmax.x, pmax.y);

		ImGui::SetNextWindowPos(pmin);

		if (p_open == true)
		{
			if (ImGui::Begin("Little pitch", &p_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground))
			{
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				drawList->AddImage((void*)(intptr_t)m_pitchImage->GetTextureId(),
					ImVec2(pmin.x+5, pmin.y+5), pmax);
				//ImGui::Image((void*)(intptr_t)m_pitchImage->GetTextureId(), ImVec2(m_pitchImage->GetWidth() * 0.4f, m_pitchImage->GetHeight() * 0.4f));
			}
			ImGui::End();
		}

	}
private:
	std::string m_pitchImagePath;
	CatolYeah::Ref<CatolYeah::Texture2D> m_pitchImage;
};

class App : public CatolYeah::Application
{
public:
	App(std::string_view assetsPath)
		:	CatolYeah::Application(assetsPath)
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
	return new App(assetsPath.string());
}