#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "Color.h"
#include "ImGuiClient.h"
#include "Client.h"

int main(int /*argc*/, char* /*argv*/[])
{
	// SDL: Initialize and create a window
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL error: " << SDL_GetError();
		return -1;
	}

	const char* title = "NavMesh Generation";
	const int width = 1300, height = 800; // Initial window size & world space size
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	SDL_Window* window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height,
			window_flags
		);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// GLEW: get function bindings (if possible)
	glewInit();
	if (!GLEW_VERSION_2_0)
	{
		std::cout << "Needs OpenGL version 3.0 or better" << std::endl;
		return -1;
	}

	// ImGUi: setup
	ImGuiClient imguiClient;
	imguiClient.Init(window, context, "#version 400");

	// Loop
	try
	{
		// Initial pathfinding points
		Point start(-500.f, -200.f);
		Point end(500.f, 200.f);

		// Imgui input variables
		bool editMode = false;
		bool showAllNodes = true;
		float weight = 1.f;
		// Path, Considered, Visited, NavMesh
		Color colors[4] = { RED, BLUE, ORANGE, GRAY };
		bool smoothPath = true;

		// Clients
		bool done = false;
		Uint32 last_ticks = SDL_GetTicks();
		Client client(window, width, height, start, end);

		while (!done)
		{
			if (editMode)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				client.Update(mouseX, mouseY);
			}

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				imguiClient.ProcessEvent(&event);
				if (imguiClient.DidConsumeInput())
					continue;

				switch (event.type)
				{
				case SDL_MOUSEBUTTONDOWN:
					client.MouseDown(event, editMode);
					break;

				case SDL_MOUSEBUTTONUP:
					client.MouseUp();
					break;

				case SDL_MOUSEMOTION:
					client.MouseMotion(event, editMode);
					break;

				case SDL_QUIT:
					done = true;
					break;

				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						done = true;
					break;

				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_RESIZED)
						client.resize(event.window.data1, event.window.data2);
					else if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
						done = true;
					break;
				}
			}

			// Compute delta time
			Uint32 ticks = SDL_GetTicks();
			double dt = 0.001 * (ticks - last_ticks);
			last_ticks = ticks;
			
			// Show ImGui and handle input
			switch (imguiClient.Draw(window, editMode, showAllNodes, weight, colors, smoothPath))
			{
			case ImGuiClient::Event::AddHole:
				client.AddHole();
				break;

			case ImGuiClient::Event::EditModePressed:
				if (!editMode)
					client.GenerateNavMesh();
				break;

			case ImGuiClient::Event::WeightChanged:
				client.SetWeight(weight);
				break;

			case ImGuiClient::Event::PathColorChanged:
				client.SetPathColor(colors[0]);
				break;

			case ImGuiClient::Event::ConsideredColorChanged:
				client.SetConsideredColor(colors[1]);
				break;

			case ImGuiClient::Event::VisitedColorChanged:
				client.SetVisitedColor(colors[2]);
				break;

			case ImGuiClient::Event::NavMeshColorChanged:
				client.SetNavMeshColor(colors[3]);
				break;

			case ImGuiClient::Event::None:
				break;
			}

			// Draw
			client.draw(dt, editMode, showAllNodes, smoothPath);
			imguiClient.RenderDrawData();

			SDL_GL_SwapWindow(window);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	// SDL: Cleanup
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}