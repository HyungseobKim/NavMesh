/*!*******************************************************************
\file		  Box.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include "Box.h"

#include "Render.h"

Box::Box(Point center, float width, float height)
	: m_width(width), m_height(height)
{
	SetPosition(center);
}

bool Box::CheckMouse(const Point& mouse) const
{
	const Point leftBottom = m_square.GetVertex(0);
	const Point rightUp = m_square.GetVertex(2);

	const float mouseX = mouse.X;
	const float	mouseY = mouse.Y;

	if (mouseX < leftBottom.X ||
		mouseX > rightUp.X ||
		mouseY < leftBottom.Y ||
		mouseY > rightUp.Y)
		return false;

	return true;
}

void Box::SetPosition(const Point& center)
{
	const float half_width = m_width / 2.f;
	const float half_height = m_height / 2.f;

	m_square.SetVertex(0, Point(center.X - half_width, center.Y - half_height));
	m_square.SetVertex(1, Point(center.X + half_width, center.Y - half_height));
	m_square.SetVertex(2, Point(center.X + half_width, center.Y + half_height));
	m_square.SetVertex(3, Point(center.X - half_width, center.Y + half_height));
}

Point Box::GetPosition() const
{
	return m_square.GetCenter();
}

void Box::IncreaseSize()
{
	Point center = m_square.GetCenter();

	m_square.SetVertex(0, Point(center.X - m_width, center.Y - m_height));
	m_square.SetVertex(1, Point(center.X + m_width, center.Y - m_height));
	m_square.SetVertex(2, Point(center.X + m_width, center.Y + m_height));
	m_square.SetVertex(3, Point(center.X - m_width, center.Y + m_height));
}

void Box::ResetSize()
{
	SetPosition(m_square.GetCenter());
}

void Box::Draw(Render* render) const
{
	render->DrawMesh(m_square);
}