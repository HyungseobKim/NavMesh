/*!*******************************************************************
\file		  SquareMesh.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include "SquareMesh.h"

const Mesh::Face SquareMesh::m_faces[2] = {
	Face(0, 1, 2),
	Face(0, 2, 3)
};

SquareMesh::SquareMesh()
	: Mesh(4, O, RED) {}

int SquareMesh::GetType() const
{
	return 1;
}

const Mesh::Face* SquareMesh::GetFaces() const
{
	return m_faces;
}

const Point* SquareMesh::GetVertices() const
{
	return m_vertices;
}

Point SquareMesh::GetVertex(int index) const
{
	return m_vertices[index];
}

void SquareMesh::SetVertex(int index, Point position)
{
	m_vertices[index] = position;
	ResetCenter(m_vertices);
}

Mesh::Face SquareMesh::GetFace(int index) const
{
	return m_faces[index];
}

int SquareMesh::GetFaceCount() const
{
	return 2;
}