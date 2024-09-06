#include "DrawingLayer.h"
#include "imgui.h"

#define OPEN_CANVAS_BUTTON_LABEL	"Open Canvas"
#define CLOSE_CANVAS_BUTTON_LABEL	"Close Canvas"

static void DrawCrossHair(const ImVec2& winPos, const ImVec2& winSize);
static void DrawPoint(const ImVec2& point, float thickness);

DrawingLayer::DrawingLayer(const std::string& pitchImagePath, std::shared_ptr<DebugObject> debugObject)
	: m_pitchImagePath(pitchImagePath), m_debugObject(debugObject)
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

	ImVec2 drawingLayerCenter = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(drawingLayerCenter, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize({(float)width, (float)height});
	if (ImGui::Begin("Drawing Layer", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground))
	{
		//if (ImGui::BeginChild("DrawCrossHair"))//ImGuiWindowFlags_NoDecoration);
		//{
		//	static std::string buttonLabel = CLOSE_CANVAS_BUTTON_LABEL;
		//	if (ImGui::Button(buttonLabel.c_str()))
		//	{
		//		p_open = !p_open;
		//		if (p_open) buttonLabel = CLOSE_CANVAS_BUTTON_LABEL;
		//		else		buttonLabel = OPEN_CANVAS_BUTTON_LABEL;
		//	}
		//}
		//ImGui::EndChild();

		auto winPos = ImGui::GetWindowPos();
		auto winSize = ImGui::GetWindowSize();
		//CY_DEBUG("Drawing layer window size: {0}x{1}", winSize.x, winSize.y);
		//CY_DEBUG("Drawing layer window pos: {0}x{1}", winPos.x, winPos.y);
	
		if (m_debugObject->DrawCrossHair)
			DrawCrossHair(winPos, winSize);

		auto imageWidth = m_pitchImage->GetWidth() * 0.4f;
		auto imageHeight = m_pitchImage->GetHeight() * 0.4f;
		ImVec2 littlePitchCenter(drawingLayerCenter.x, (float)height - imageHeight / 2.0f - 10);

		ImVec2 pmin = ImVec2(0.5f * ((float)width - imageWidth) + winPos.x, (float)height - imageHeight - 10 + winPos.y);
		ImVec2 pmax = ImVec2(pmin.x + imageWidth, pmin.y + imageHeight);
		//CY_DEBUG("pmin: {0}x{1} / pmax: {2}x{3}", pmin.x, pmin.y, pmax.x, pmax.y);
		if (m_debugObject->LittlePitchPoints)
		{
			DrawPoint(pmin, 5);
			DrawPoint(pmax, 5);
		}

		if (p_open == true)
		{
			//if (ImGui::Begin("Little pitch", &p_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground))
			/*if (ImGui::BeginChild("Campograma", { imageWidth, imageHeight }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground))
			{*/
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				drawList->AddImage((void*)(intptr_t)m_pitchImage->GetTextureId(),
					pmin, pmax, ImVec2(0,0), ImVec2(1,1), ImGui::GetColorU32(IM_COL32(255, 255, 255, 100)));
				//ImGui::Image((void*)(intptr_t)m_pitchImage->GetTextureId(), ImVec2(m_pitchImage->GetWidth() * 0.4f, m_pitchImage->GetHeight() * 0.4f));
			/*}
			ImGui::EndChild();*/
		}
	}
	ImGui::End();
}

bool DrawingLayer::m_OnWindowResize(CatolYeah::WindowResizeEvent& windowResizeEvent)
{
	width = windowResizeEvent.GetWindowWidth();
	height = windowResizeEvent.GetWindowHeight();
	return EVENT_RETURN_PASS_ON;
}

static void DrawCrossHair(const ImVec2 &winPos, const ImVec2& winSize)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 horizontalLineP1(winPos.x, winPos.y + winSize.y / 2.0f);
	ImVec2 horizontalLineP2(winPos.x + winSize.x, winPos.y + winSize.y / 2.0f);
	ImVec2 verticalLineP1(winPos.x + winSize.x / 2.0f, winPos.y);
	ImVec2 verticalLineP2(winPos.x + winSize.x / 2.0f, winPos.y + winSize.y);
	drawList->AddLine(horizontalLineP1, horizontalLineP2,
		ImGui::GetColorU32(IM_COL32(255, 0, 0, 255)),
		3.0f);
	drawList->AddLine(verticalLineP1, verticalLineP2,
		ImGui::GetColorU32(IM_COL32(255, 0, 0, 255)),
		3.0f);
}

static void DrawPoint(const ImVec2& point, float thickness)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddCircleFilled(point, thickness, ImGui::GetColorU32(IM_COL32(255, 0, 0, 255)));
}