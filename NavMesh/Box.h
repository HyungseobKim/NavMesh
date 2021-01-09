/*!*******************************************************************
\headerfile   Box.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Box class being used for UI.
********************************************************************/
#ifndef BOX_H
#define BOX_H

#include "SquareMesh.h"
#include "Color.h"

class Render;

/*!*******************************************************************
\class Box
\brief
	   Box is used as UI in this framework, so it has methods for that
	   like handle mouse input, moving box and setting size.
********************************************************************/
class Box
{
public:
	/*!*******************************************************************
	\brief
		   Constructor that initializes position and size.

	\param center
		   Center point of box. Default is (0, 0).

	\param width
		   Width of the box. Default is 10.

	\param height
		   Height of the box. Default is 10.
	********************************************************************/
	Box(Point center = O, float width = 10.f, float height = 10.f);

	/*!*******************************************************************
	\brief
		   Check whether mouse is hovering on this box.

	\param mouse
		   Position of mouse.

	\return bool
			If position is inside box, returns true.
			Otherwise, returns false.
	********************************************************************/
	bool CheckMouse(const Point& mouse) const;
	/*!*******************************************************************
	\brief
		   Move box to the given position.

	\param center
		   New center.
	********************************************************************/
	void SetPosition(const Point& center);
	/*!*******************************************************************
	\brief
		   Getter method for position of box.

	\return Point
			Center of the box.
	********************************************************************/
	Point GetPosition() const;

	/*!*******************************************************************
	\brief
		   Increase size of box twice.
		   It will be used when user is hovering their mouse on this box,
		   so user can notice which vertex they are looking for easily.
	********************************************************************/
	void IncreaseSize();
	/*!*******************************************************************
	\brief
		   Set size to default when mouse is not hovering anymore.
	********************************************************************/
	void ResetSize();

	/*!*******************************************************************
	\brief
		   Draws the box.

	\param render
		   Render that has drawing methods.
	********************************************************************/
	void Draw(Render* render) const;

private:
	SquareMesh m_square; //!< Mesh of box, so holding information about vertices and color.

	const float m_width; //!< Width of the box.
	const float m_height; //!< Height of the box.
};

#endif // BOX_H
