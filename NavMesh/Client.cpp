/*!*******************************************************************
\file   Client.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include <sstream>
#include <iostream>

#include "Client.h"

#include "TriangleMesh.h"
#include "SquareMesh.h"
#include "Box.h"

Client::Client(SDL_Window* window, int width, int height, const Point& start, const Point& end)
	: m_window(window), m_width(width), m_height(height), m_world(width, height, start, end)
{
	m_render = new Render(static_cast<float>(width), static_cast<float>(height));
}

Client::~Client()
{
	delete m_render;
}

void Client::Update(int mouseX, int mouseY)
{
	m_world.Update(ConvertMousePosition(Point(static_cast<float>(mouseX), static_cast<float>(mouseY))));
}

void Client::draw(double dt, bool editMode, bool showAllNodes, bool smoothPath)
{
	// Show fps with title
	++m_frame_count;
	m_frame_time += dt;
	if (m_frame_time >= 0.1)
	{
		double fps = m_frame_count / m_frame_time;
		m_frame_count = 0;
		m_frame_time = 0;

		std::stringstream ss;
		ss << "NavMesh Project [fps=" << static_cast<int>(fps) << "]";
		SDL_SetWindowTitle(m_window, ss.str().c_str());
	}

	// Draw
	m_render->ClearBuffers();
	m_world.Draw(m_render, editMode, showAllNodes, smoothPath);
}

void Client::resize(int width, int height)
{
	glViewport(0, 0, width, height);
	
	m_width = width;
	m_height = height;
}

Point Client::ConvertMousePosition(const Point& mouse) const
{
	// SDL space to viewport
	Point result;
	result.X = mouse.X - m_width / 2.f;
	result.Y = mouse.Y * -1.f + m_height / 2.f;

	// Normalize. LeftBottom = (-1, -1), RightTop = (1, 1)
	result.X /= static_cast<float>(m_width);
	result.Y /= static_cast<float>(m_height);

	return result;
}

void Client::MouseMotion(const SDL_Event& event, bool editMode)
{
	if (editMode)
	{
		const auto& mouse = event.button;
		m_world.MouseMotion(ConvertMousePosition(Point(static_cast<float>(mouse.x), static_cast<float>(mouse.y))));
	}
}

void Client::MouseUp()
{
	m_world.MouseUp();
}

void Client::MouseDown(const SDL_Event& event, bool editMode)
{
	bool left = (event.button.button == SDL_BUTTON_LEFT);

	if (editMode)
		m_world.MouseDown(left);
	else // Set mouse position as path vertex.
	{
		const auto& mouse = event.button;
		const Point pos = ConvertMousePosition(Point(static_cast<float>(mouse.x), static_cast<float>(mouse.y)));

		if (left)
			m_world.SetStartPoint(pos);
		else
			m_world.SetEndPoint(pos);
	}
}

void Client::AddHole()
{
	m_world.AddHole();
}

void Client::GenerateNavMesh()
{
	m_world.GenerateNavMesh();
}

void Client::SetWeight(float weight)
{
	m_world.SetWeight(weight);
}

void Client::SetConsideredColor(const Color& color)
{
	m_world.SetConsideredColor(color);
}

void Client::SetVisitedColor(const Color& color)
{
	m_world.SetVisitedColor(color);
}

void Client::SetPathColor(const Color& color)
{
	m_world.SetPathColor(color);
}

void Client::SetNavMeshColor(const Color& color)
{
	m_world.SetNavMeshColor(color);
}