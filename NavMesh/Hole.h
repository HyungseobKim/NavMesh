/*!*******************************************************************
\headerfile   Hole.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Hole class being used as wall for the
			  geography.
********************************************************************/
#ifndef HOLE_H
#define HOLE_H

#include "Box.h"
#include "TriangleMesh.h"

class Render;

/*!*******************************************************************
\class Hole
\brief
	   Hole is used as wall, and having methods for editor.
********************************************************************/
class Hole
{
public:
	/*!*******************************************************************
	\brief
		   Constructor that align boxes to each vertex.
	********************************************************************/
	Hole();
	/*!*******************************************************************
	\brief
		   Default assign constructor.
	********************************************************************/
	Hole(const Hole& rhs) = default;
	/*!*******************************************************************
	\brief
		   Assign opeator that copies variables.

	\param rhs
		   Hole to copy from.

	\return Hole&
			Return this object.
	********************************************************************/
	Hole& operator=(const Hole& rhs);

	/*!*******************************************************************
	\brief
		   Check whether mouse is hovering on one of the UI boxes of this
		   hole.

	\param mouse
		   Position of mouse.

	\return bool
			If there is a box intersecting with mouse, returns true.
			If there is no one, returns false.
	********************************************************************/
	bool CheckMouse(const Point& mouse);
	/*!*******************************************************************
	\brief
		   Draws the hole and UI boxes depends on mode.

	\param render
		   Render that has drawing methods.

	\param editMode
		   If it is true, draws UI boxes too.
	********************************************************************/
	void Draw(Render* render, bool editMode) const;

	/*!*******************************************************************
	\brief
		   Move hole and UI boxes.

	\param position
		   New center position of hole.
	********************************************************************/
	void MoveHole(const Point& position);

	/*!*******************************************************************
	\brief
		   Determine and return vertices in clockwise order.

	\param points
		   Array of vertices in clockwise order.
	********************************************************************/
	void GetClockwiseVertices(Point (&points)[3]) const;

private:
	/*!*******************************************************************
	\brief
		   Move three boxes to each vertex of hole.
		   Move one box to the center of the hole.
	********************************************************************/
	void AlignBoxes();

private:
	TriangleMesh m_triangle; //!< Mesh of hole, so holding information about vertices and color.

	Box m_center;  //!< UI box for center of the hole.
	Box m_boxes[3]; //!< UI boxes for each vertex of the hole.
	int m_picked = -1; //!< Index of the box currently intersecting with mouse.
};

#endif // !HOLE_H
