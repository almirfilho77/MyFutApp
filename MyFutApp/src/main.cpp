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

		CatolYeah::RenderCommand::Clear();

		ImGui::ShowDemoWindow(&show);

		ImGui::Begin("GPU Info");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);
		ImGui::End();
	}
};

class DrawingLayer : public CatolYeah::Layer
{
public:
	DrawingLayer() {}

	void OnImGuiRender() override
	{
		static bool p_open = true;

		CatolYeah::RenderCommand::Clear();

		ImGui::Begin("Open/Close Canvas", 0, ImGuiWindowFlags_NoDecoration);
		static std::string buttonLabel = CLOSE_CANVAS_BUTTON_LABEL;
		if (ImGui::Button(buttonLabel.c_str()))
		{
			p_open = !p_open;
			if (p_open) buttonLabel = CLOSE_CANVAS_BUTTON_LABEL;
			else		buttonLabel = OPEN_CANVAS_BUTTON_LABEL;
		}
		ImGui::End();

		if (p_open == true)
		{
			if (ImGui::Begin("Drawing area", &p_open, ImGuiWindowFlags_NoCollapse))
			{
				// Begin of example code to test and modify
				static ImVector<ImVec2> points;
				static ImVec2 scrolling(0.0f, 0.0f);
				static bool opt_enable_grid = true;
				static bool opt_enable_context_menu = true;
				static bool adding_line = false;

				// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
				ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
				ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
				if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
				if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
				ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

				// Draw border and background color
				ImGuiIO& io = ImGui::GetIO();
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
				draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

				// This will catch our interactions
				ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
				const bool is_hovered = ImGui::IsItemHovered(); // Hovered
				const bool is_active = ImGui::IsItemActive();   // Held
				const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
				const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

				// Add first and second point
				if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					points.push_back(mouse_pos_in_canvas);
					points.push_back(mouse_pos_in_canvas);
					adding_line = true;
				}
				if (adding_line)
				{
					points.back() = mouse_pos_in_canvas;
					if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
						adding_line = false;
				}

				// Draw grid + all lines in the canvas
				draw_list->PushClipRect(canvas_p0, canvas_p1, true);
				if (opt_enable_grid)
				{
					const float GRID_STEP = 64.0f;
					for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
						draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
					for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
						draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
				}
				for (int n = 0; n < points.Size; n += 2)
					draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
				draw_list->PopClipRect();

				// End of example code //
			}
			ImGui::End();
		}

	}
};

class App : public CatolYeah::Application
{
public:
	App(std::string_view assetsPath)
		:	CatolYeah::Application(assetsPath)
	{
		CY_DEBUG("Init App class");
		CY_DEBUG("Assets path: {0}", assetsPath);

		PushOverlay(new DrawingLayer());
		PushOverlay(new DebugLayer());
	}
};

CatolYeah::Application* CatolYeah::CreateApplication()
{
	fs::path assetsPath = fs::current_path() / "assets" / "shaders";
	return new App(assetsPath.string());
}