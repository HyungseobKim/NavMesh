/*!*******************************************************************
\file         SearchingArea.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include "SearchingArea.h"

#include "Render.h"

SearchingArea::SearchingArea(float width, float height, const Point& start, const Point& end)
	: m_width(width), m_height(height),	m_boundary{ 
		Point(-width /2.f, -height /2.f), 
		Point(width /2.f, -height /2.f), 
		Point(width /2.f, height /2.f), 
		Point(-width /2.f, height /2.f) 
	}, m_astar(start, end)
{
	AddHole();
	GenerateNavMesh();
}

void SearchingArea::Update(const Point& mouse)
{
	m_picked = -1;
	const int size = static_cast<int>(m_holes.size());

	for (int i = 0; i < size; ++i)
	{
		if (m_holes[i].CheckMouse(mouse))
			m_picked = i;
	}
}

void SearchingArea::Draw(Render* render, bool editMode, bool showAllNodes, bool smoothPath) const
{
	// Draw boundary
	for (int i = 0; i < 4; ++i)
		render->DrawLine(m_boundary[i], m_boundary[(i + 1) % 4], BLACK);

	// Draw holes
	for (auto& hole : m_holes)
		hole.Draw(render, editMode);

	// If it is not edit mode,
	if (!editMode)
	{
		// draw nav meshes and path
		m_navmesh.Draw(render);
		m_astar.DrawPath(render, showAllNodes, smoothPath);
	}
}

void SearchingArea::AddHole()
{
	m_holes.push_back(Hole());
}

void SearchingArea::RemoveHole()
{
	if (m_picked < 0) return;

	m_holes.erase(m_holes.begin() + m_picked);
	m_picked = -1;
}

void SearchingArea::MoveHole(const Point& position)
{
	if (m_picked < 0) return;

	m_holes[m_picked].MoveHole(position);
}

void SearchingArea::GenerateNavMesh()
{
	const int size = static_cast<int>(m_holes.size());

	// Boundary + number of holes * 3 (triangle)
	std::vector<NavMeshManager::Vertex> vertices;
	vertices.resize(4 + size * 3);

	// Inserts vertices of boundary
	for (int i = 0; i < 4; ++i)
	{
		auto& curr = vertices[i];

		curr.m_position = m_boundary[i];
		curr.m_next = &vertices[(i + 1) % 4];
		curr.m_next->m_prev = &curr;
	}

	// Inserts vertices of holes
	Point points[3];
	for (int i = 0; i < size; ++i)
	{
		m_holes[i].GetClockwiseVertices(points);

		const int start = 4 + i * 3;
		for (int j = start; j < start + 3; ++j)
		{
			auto& curr = vertices[j];

			curr.m_position = points[j - start];
			curr.m_next = &vertices[(j - start + 1) % 3 + start];
			curr.m_next->m_prev = &curr;
		}
	}

	// Makes new nav meshes.
	m_navmesh.Generate(vertices);
	// After that, find path newly.
	m_astar.InitializeMap(m_navmesh.GetNavMeshes());
}

void SearchingArea::SetStartPoint(const Point& start)
{
	m_astar.SetStart(start);
}

void SearchingArea::SetEndPoint(const Point& end)
{
	m_astar.SetEnd(end);
}

void SearchingArea::SetWeight(float weight)
{
	m_astar.SetWeight(weight);
}

void SearchingArea::SetConsideredColor(const Color& color)
{
	m_astar.SetConsideredColor(color);
}

void SearchingArea::SetVisitedColor(const Color& color)
{
	m_astar.SetVisitedColor(color);
}

void SearchingArea::SetPathColor(const Color& color)
{
	m_astar.SetPathColor(color);
}

void SearchingArea::SetNavMeshColor(const Color& color)
{
	m_navmesh.SetColor(color);
}