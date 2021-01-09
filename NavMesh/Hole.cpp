#include <algorithm>

#include "Hole.h"

#include "Render.h"

Hole::Hole()
{
	AlignBoxes();
}

Hole& Hole::operator=(const Hole& rhs)
{
	m_triangle = rhs.m_triangle;

	m_center.SetPosition(rhs.m_center.GetPosition());
	for (int i = 0; i < 3; ++i)
		m_boxes[i].SetPosition(rhs.m_boxes[i].GetPosition());

	m_picked = rhs.m_picked;

	return *this;
}

bool Hole::CheckMouse(const Point& mouse)
{
	// Reset
	if (m_picked >= 0)
	{
		if (m_picked == 3)
			m_center.ResetSize();
		else
			m_boxes[m_picked].ResetSize();

		m_picked = -1;
	}
	
	// Special case center
	if (m_center.CheckMouse(mouse))
	{
		m_picked = 3;
		m_center.IncreaseSize();
		return true;
	}

	// Check vertices
	for (int i = 0; i < 3; i++)
	{
		Box& curr = m_boxes[i];
		if (curr.CheckMouse(mouse))
		{
			m_picked = i;
			curr.IncreaseSize();
			return true;
		}
	}

	// No collision on boxes
	m_picked = -1;
	return false;
}

void Hole::Draw(Render* render, bool editMode) const
{
	render->DrawMesh(m_triangle);

	if (!editMode) return;
	m_center.Draw(render);

	for (auto& box : m_boxes)
		box.Draw(render);
}

void Hole::AlignBoxes()
{
	m_center.SetPosition(m_triangle.GetCenter());

	const auto vertices = m_triangle.GetVertices();

	m_boxes[0].SetPosition(vertices[0]);
	m_boxes[1].SetPosition(vertices[1]);
	m_boxes[2].SetPosition(vertices[2]);
}

void Hole::MoveHole(const Point& position)
{
	if (m_picked < 0) return;

	// Special case center box
	if (m_picked == 3)
	{
		const Vector delta = position - m_triangle.GetCenter();

		const auto& vertices = m_triangle.GetVertices();
		for (int i = 0; i < 3; ++i)
			m_triangle.SetVertex(i, vertices[i] + delta);

		AlignBoxes();
		return;
	}

	m_triangle.SetVertex(m_picked, position);
	AlignBoxes();
}

void Hole::GetClockwiseVertices(Point(&points)[3]) const
{
	auto vertices = m_triangle.GetVertices();

	int left = 0;
	int right = 1;

	// Check left vertex is really left. If is not, swap.
	if (vertices[left].X > vertices[right].X)
		std::swap(left, right);

	// Make equation of line.
	const auto& posL = vertices[left];
	const auto& posR = vertices[right];

	const float dx = posR.X - posL.X;
	const float dy = posR.Y - posL.Y;
	const float c = dy * posR.X - dx * posR.Y;

	// Check the other vertex is whether on the above of the line.
	const bool above = (dy * vertices[2].X - dx * vertices[2].Y - c) < 0;

	points[0] = vertices[left];
	points[1] = vertices[right];
	points[2] = vertices[2];

	// If it is above, swap the order.
	if (above)
		std::swap(points[0], points[1]);
}