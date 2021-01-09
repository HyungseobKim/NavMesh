/*!*******************************************************************
\headerfile   SquareMesh.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for SquareMesh class which defines square.
********************************************************************/
#ifndef SQUAREMESH_H
#define SQUAREMESH_H

#include "Mesh.h"

/*!*******************************************************************
\class SquareMesh
\brief
	   Having four vertices and overriding methods that inherited
	   from Mesh class.
********************************************************************/
class SquareMesh : public Mesh
{
public:
	/*!*******************************************************************
	\brief
		   Constructor that calls constructor of base class.
	********************************************************************/
	SquareMesh();

	/*!*******************************************************************
	\brief
		   Getter method for the vertex corresponds to given index.

	\param index
		   Index of vertex.

	\return Point
			Vertex corresponds to given index.
	********************************************************************/
	Point GetVertex(int index) const override;
	/*!*******************************************************************
	\brief
		   Setter method for position of specific vertex.

	\param index
		   Index of vertex to change.

	\param position
		   New position of the vertex.
	********************************************************************/
	void SetVertex(int index, Point position) override;

	/*!*******************************************************************
	\brief
		   Getter method for the face corresponds to the given index.

	\param index
		   Index of face.

	\return Face
			Face corresponds to given index.
	********************************************************************/
	Face GetFace(int index) const override;
	/*!*******************************************************************
	\brief
		   Getter method for the number of faces.

	\return int
			The number of faces of this mesh.
	********************************************************************/
	int GetFaceCount() const override;

	/*!*******************************************************************
	\brief
		   Getter method for the type of this mesh.

	\return int
			Returns 1, which means this mesh is square.
	********************************************************************/
	int GetType() const override;

	/*!*******************************************************************
	\brief
		   Getter method for the all vertices of this mesh.

	\return const Point*
			Array of const points.
	********************************************************************/
	const Point* GetVertices() const override;
	/*!*******************************************************************
	\brief
		   Pure virtual getter method for the all faces of this mesh.

	\return const Face*
			Array of const faces.
	********************************************************************/
	const Face* GetFaces() const override;

private:
	Point m_vertices[4] = { Point(-1.f , -1.f), Point(1.f, -1.f), Point(1.f, 1.f), Point(-1.f, 1.f) }; //!< The vertices of this mesh.
	static const Face m_faces[2]; //!< The faces of this mesh.
};

#endif // !SQUAREMESH_H
