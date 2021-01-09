/*!*******************************************************************
\file		  Mesh.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include "Mesh.h"

int Mesh::GetVertexCount() const
{
	return m_vertexCount;
}

Point Mesh::GetCenter() const
{
	return m_center;
}

void Mesh::ResetCenter(Point* vertices)
{
	Point p = O;

	// Find average of all vertices.
	for (int i = 0; i < m_vertexCount; ++i)
		p += vertices[i];

	m_center = p / m_vertexCount;
}

Color Mesh::GetColor() const
{
	return m_color;
}

void Mesh::SetColor(const Color& color)
{
	m_color = color;
}