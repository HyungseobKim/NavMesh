#include "World.h"

#include "Render.h"

World::World(int width, int height, const Point& start, const Point& end)
	: m_width(width), m_height(height), 
	m_area(width * 0.97f, height * 0.97f, start, end)
{}

Point World::ToWorldCoordinate(const Point& point) const
{
	Point result = point;

	// Scale to world space
	result.X *= static_cast<float>(m_width);
	result.Y *= static_cast<float>(m_height);

	return result;
}

void World::Update(Point mouse)
{
	m_area.Update(ToWorldCoordinate(mouse));
}

void World::Draw(Render* render, bool editMode, bool showAllNodes, bool smoothPath) const
{
	m_area.Draw(render, editMode, showAllNodes, smoothPath);
}

void World::MouseMotion(Point mouse)
{
	if (!m_isMouseDown) return;

	m_area.MoveHole(ToWorldCoordinate(mouse));
}

void World::MouseUp()
{
	m_isMouseDown = false;
}

void World::MouseDown(bool left)
{
	if (left)
		m_isMouseDown = true;
	else
		m_area.RemoveHole();
}

void World::AddHole()
{
	m_area.AddHole();
}

void World::GenerateNavMesh()
{
	m_area.GenerateNavMesh();
}

void World::SetStartPoint(const Point& start)
{
	m_area.SetStartPoint(ToWorldCoordinate(start));
}

void World::SetEndPoint(const Point& end)
{
	m_area.SetEndPoint(ToWorldCoordinate(end));
}

void World::SetWeight(float weight)
{
	m_area.SetWeight(weight);
}

void World::SetConsideredColor(const Color& color)
{
	m_area.SetConsideredColor(color);
}

void World::SetVisitedColor(const Color& color)
{
	m_area.SetVisitedColor(color);
}

void World::SetPathColor(const Color& color)
{
	m_area.SetPathColor(color);
}

void World::SetNavMeshColor(const Color& color)
{
	m_area.SetNavMeshColor(color);
}