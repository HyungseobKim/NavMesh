/*!*******************************************************************
\file		  TriangleMesh.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include "TriangleMesh.h"

const Mesh::Face TriangleMesh::m_face = Mesh::Face(0, 1, 2);

TriangleMesh::TriangleMesh()
	: Mesh(3, O, BLACK)
{
	ResetCenter(m_vertices);
}

TriangleMesh& TriangleMesh::operator=(const TriangleMesh& rhs)
{
	for (int i = 0; i < 3; ++i)
		m_vertices[i] = rhs.m_vertices[i];

	ResetCenter(m_vertices);
	SetColor(rhs.GetColor());

	return *this;
}

int TriangleMesh::GetType() const
{
	return 0;
}

const Mesh::Face* TriangleMesh::GetFaces() const
{
	return &m_face;
}

const Point* TriangleMesh::GetVertices() const
{
	return m_vertices;
}

void TriangleMesh::SetVertex(int index, Point position)
{
	m_vertices[index] = position;
	ResetCenter(m_vertices);
}

Point TriangleMesh::GetVertex(int index) const
{
	return m_vertices[index];
}

Mesh::Face TriangleMesh::GetFace(int /*index*/) const
{
	return m_face;
}

int TriangleMesh::GetFaceCount() const
{
	return 1;
}