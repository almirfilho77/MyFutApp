#include "DrawingLayer.h"
#include "imgui.h"

DrawingLayer::DrawingLayer(const std::string& pitchImagePath)
	: m_pitchImagePath(pitchImagePath)
{
	width = CatolYeah::Application::Get().GetWindow().GetWidth();
	height = CatolYeah::Application::Get().GetWindow().GetHeight();
}

void DrawingLayer::OnAttach()
{
	m_pitchImage = CatolYeah::Texture2D::Create(m_pitchImagePath);
}

void DrawingLayer::OnEvent(CatolYeah::Event& event)
{
	CatolYeah::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<CatolYeah::WindowResizeEvent>(CY_BIND_EVENT_FN(DrawingLayer::m_OnWindowResize));
}

void DrawingLayer::OnImGuiRender()
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
				ImVec2(pmin.x + 5, pmin.y + 5), pmax);
			//ImGui::Image((void*)(intptr_t)m_pitchImage->GetTextureId(), ImVec2(m_pitchImage->GetWidth() * 0.4f, m_pitchImage->GetHeight() * 0.4f));
		}
		ImGui::End();
	}
}

bool DrawingLayer::m_OnWindowResize(CatolYeah::WindowResizeEvent& windowResizeEvent)
{
	width = windowResizeEvent.GetWindowWidth();
	height = windowResizeEvent.GetWindowHeight();
	return EVENT_RETURN_PASS_ON;
}