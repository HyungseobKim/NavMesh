/*!*******************************************************************
\headerfile   Mesh.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Mesh class which stores basic
			  information of meshes.
********************************************************************/
#ifndef MESH_H
#define MESH_H

#include "Point.h"
#include "Color.h"

/*!*******************************************************************
\class Mesh
\brief
	   Has vertices, center and color of this mesh.
	   There are some methods to set or get those values too.
********************************************************************/
class Mesh
{
public:
	/*!*******************************************************************
	\struct Face
	\brief
		   Face of mesh.
	********************************************************************/
	struct Face
	{
		unsigned int index1, index2, index3;
		Face(unsigned int i1, unsigned int i2, unsigned int i3)
			: index1(i1), index2(i2), index3(i3) {}
	};

	/*!*******************************************************************
	\brief
		   Constructor that initializes shape, position and color.

	\param count
		   The number of vertices.

	\param center
		   Position of center point.

	\param color
		   Color of this mesh.
	********************************************************************/
	Mesh(int count, Point center, Color color)
		: m_vertexCount(count), m_center(center), m_color(color) {}

	/*!*******************************************************************
	\brief
		   Virtual destructor does nothing by default.
	********************************************************************/
	virtual ~Mesh() {}
	
	/*!*******************************************************************
	\brief
		   Getter method for the number of vertex.

	\return int
			Returns m_vertexCount.
	********************************************************************/
	int GetVertexCount() const;
	/*!*******************************************************************
	\brief
		   Getter method for the position of center point.

	\return Point
			Returns m_center.
	********************************************************************/
	Point GetCenter() const;

	/*!*******************************************************************
	\brief
		   Getter method for the color of this mesh.

	\return Color
			Returns m_color.
	********************************************************************/
	Color GetColor() const;
	/*!*******************************************************************
	\brief
		   Setter method for color of this mesh.

	\param color
		   New color.
	********************************************************************/
	void SetColor(const Color& color);

	/*!*******************************************************************
	\brief
		   Pure virtual getter method for the corresponding vertex to the 
		   given index.

	\param index
		   Index of vertex.

	\return Point
			Vertex corresponds to given index.
	********************************************************************/
	virtual Point GetVertex(int index) const = 0;
	/*!*******************************************************************
	\brief
		   Pure virtual setter method for position of specific vertex.

	\param index
		   Index of vertex to change.

	\param position
		   New position of the vertex.
	********************************************************************/
	virtual void SetVertex(int index, Point position) = 0;
	
	/*!*******************************************************************
	\brief
		   Pure virtual getter method for the face corresponds to the
		   given index.

	\param index
		   Index of face.

	\return Face
			Face corresponds to given index.
	********************************************************************/
	virtual Face GetFace(int index) const = 0;
	/*!*******************************************************************
	\brief
		   Pure virtual getter method for the number of faces.

	\return int
			The number of faces of this mesh.
	********************************************************************/
	virtual int GetFaceCount() const = 0;

	/*!*******************************************************************
	\brief
		   Pure virtual getter method for the type of this mesh.

	\return int
			Returns 0, if it is triangle.
			Returns 1, if it is square.
	********************************************************************/
	virtual int GetType() const = 0;

	/*!*******************************************************************
	\brief
		   Pure virtual getter method for the all vertices of this mesh.

	\return const Point*
			Array of const points.
	********************************************************************/
	virtual const Point* GetVertices() const = 0;
	/*!*******************************************************************
	\brief
		   Pure virtual getter method for the all faces of this mesh.

	\return const Face*
			Array of const faces.
	********************************************************************/
	virtual const Face* GetFaces() const = 0;

protected:
	/*!*******************************************************************
	\brief
		   Helper method for child classes which re-computes the position
		   of center depends on given vertices.

	\param vertices
		   Array of vertices.
	********************************************************************/
	void ResetCenter(Point* vertices);

private:
	const int m_vertexCount; //!< The number of vertices.
	Point m_center; //!< The center of this mesh. The average of all vertices.
	Color m_color; //!< The color of this mesh.
};

#endif // !MESH_H
