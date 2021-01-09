/*!*******************************************************************
\headerfile   TriangleMesh.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for TriangleMesh class which defines
			  triangle.
********************************************************************/
#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "Mesh.h"

/*!*******************************************************************
\class TriangleMesh
\brief
	   Having three vertices and overriding methods that inherited
	   from Mesh class.
********************************************************************/
class TriangleMesh : public Mesh
{
public:
	/*!*******************************************************************
	\brief
		   Constructor that calls constructor of base class.
	********************************************************************/
	TriangleMesh();

	TriangleMesh& operator=(const TriangleMesh& rhs);

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
	Face GetFace(int index = 0) const override;
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
	Point m_vertices[3] = { Point(-50.f, -50.f), Point(50.f, -50.f), Point(0.f, 50.f) }; //!< The vertices of this mesh.
	static const Face m_face; //!< The faces of this mesh.
};

#endif // !TRIANGLEMESH_H
