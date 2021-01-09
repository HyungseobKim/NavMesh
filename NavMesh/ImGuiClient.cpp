/*!*******************************************************************
\file		  ImGuiClient.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for abstracted ImGuiClient class which
			  manages ImGui.
********************************************************************/
#include "ImGuiClient.h"

ImGuiClient::ImGuiClient()
{
	// ImGUi: setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImGui::StyleColorsDark();
}

void ImGuiClient::Init(SDL_Window* window, SDL_GLContext& context, const char* glsl_version)
{
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiClient::ProcessEvent(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}

ImGuiClient::Event ImGuiClient::Draw(SDL_Window* window, bool& editMode, bool& showAllNodes, float& weight, Color(&color)[4], bool& smoothPath)
{
	Event event = Event::None;

	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	// Show default winodw
	ImGui::Begin("Control Panel");

	if (ImGui::Checkbox("Edit Mode", &editMode))
		event = Event::EditModePressed;

	ImGui::Text("Left Mouse Click: Set start point");
	ImGui::Text("Right Mouse Click: Set end point");

	if (ImGui::SliderFloat("Weight", &weight, 0.f, 2.f))
		event = Event::WeightChanged;

	ImGui::Checkbox("Path smoothing", &smoothPath);
	ImGui::Checkbox("Show all nodes considered", &showAllNodes);

	if (ImGui::ColorEdit3("Path", (float*)&color[0]))
		event = Event::PathColorChanged;

	if (ImGui::ColorEdit3("Considered Nodes", (float*)&color[1]))
		event = Event::ConsideredColorChanged;

	if (ImGui::ColorEdit3("Visited Nodes", (float*)&color[2]))
		event = Event::VisitedColorChanged;

	if (ImGui::ColorEdit3("Nav Mesh Nodes", (float*)&color[3]))
		event = Event::NavMeshColorChanged;

	ImGui::End();

	// Show Editor
	if (editMode)
	{
		ImGui::Begin("Editor");

		ImGui::Text("Drag boxes to move vertex or triangle.");
		ImGui::Text("Right mouse click on boxes to remove.");

		ImGui::NewLine();

		if (ImGui::Button("Add Hole"))
			event = Event::AddHole;

		ImGui::SameLine();
		ImGui::Text("Add triangle at the center");

		ImGui::End();
	}

	// Draw ImGui
	ImGui::Render();

	return event;
}

void ImGuiClient::RenderDrawData()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiClient::DidConsumeInput()
{
	return ImGui::GetIO().WantCaptureMouse;
}

ImGuiClient::~ImGuiClient()
{
	// ImGui: Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}