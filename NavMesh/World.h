/*!*******************************************************************
\headerfile   SearchingArea.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for World class which defines and manages
			  the world space.
********************************************************************/
#ifndef WORLD_H
#define WORLD_H

#include "SearchingArea.h"

class Render;

/*!*******************************************************************
\class World
\brief
	   Having abstracred methods and methods for transforming points
	   to world space.
********************************************************************/
class World
{
public:
	/*!*******************************************************************
	\brief
		   Constructor that initializes the size of world and two vertices
		   of path.

	\param width
		   Width of world.

	\param height
		   Height of world.

	\param start
		   Beginning point of path.

	\param end
		   Destination point of path.
	********************************************************************/
	World(int width, int height, const Point& start, const Point& end);

	/*!*******************************************************************
	\brief
		   Abstracted update method.
		   Transforms mouse position and call related update method of
		   object under hierarchy.

	\param mouse
		   Poisition of mouse needs to transform to the world space.
	********************************************************************/
	void Update(Point mouse);
	/*!*******************************************************************
	\brief
		   Abstracted draw method.
		   Call related draw method of object under hierarchy.

	\param render
		   Render that has drawing methods.

	\param editMode
		   Passing value to the underlying object.

	\param showAllNodes
		   Passing value to the underlying object.

	\param smoothPath
		   Passing value to the underlying object.
	********************************************************************/
	void Draw(Render* render, bool editMode, bool showAllNodes, bool smoothPath) const;

	/*!*******************************************************************
	\brief
		   Abstracted handling mouse motion method.
		   If mouse is down, call related mouse motion method of object
		   under hierarchy.

	\param mouse
		   Poisition of mouse needs to transform to the world space.
	********************************************************************/
	void MouseMotion(Point mouse);
	/*!*******************************************************************
	\brief
		   Setter method for mouse status to be up.
	********************************************************************/
	void MouseUp();
	/*!*******************************************************************
	\brief
		   Called when mouse is pressed.
		   If button is left, set m_isMouseDown as true.
		   If button is right, call remove hole method of object under
		   hierarchy.

	\param left
		   If true, pressed mouse button is left.
		   If false, pressed mouse button is right.
	********************************************************************/
	void MouseDown(bool left);

	/*!*******************************************************************
	\brief
		   Abstracted method for handling editor input.
		   Call related method of object under hierarchy.
	********************************************************************/
	void AddHole();
	/*!*******************************************************************
	\brief
		   Abstracted method called when user escapes edit mode.
		   Call related method of object under hierarchy.
	********************************************************************/
	void GenerateNavMesh();

	/*!*******************************************************************
	\brief
		   Abstracted setter method for start vertex of path.

	\param start
		   Position of start vertex.
	********************************************************************/
	void SetStartPoint(const Point& start);
	/*!*******************************************************************
	\brief
		   Abstracted setter method for end vertex of path.

	\param end
		   Position of end vertex.
	********************************************************************/
	void SetEndPoint(const Point& end);
	/*!*******************************************************************
	\brief
		   Abstracted setter method for weight.

	\param weight
		   New weight.
	********************************************************************/
	void SetWeight(float weight);

	/*!*******************************************************************
	\brief
		   Abstracted setter method for color of considered nodes.

	\param color
		   New color.
	********************************************************************/
	void SetConsideredColor(const Color& color);
	/*!*******************************************************************
	\brief
		   Abstracted setter method for color of visited nodes.

	\param color
		   New color.
	********************************************************************/
	void SetVisitedColor(const Color& color);
	/*!*******************************************************************
	\brief
		   Abstracted setter method for color of path.

	\param color
		   New color.
	********************************************************************/
	void SetPathColor(const Color& color);
	/*!*******************************************************************
	\brief
		   Abstracted setter method for color of navigation meshes.

	\param color
		   New color.
	********************************************************************/
	void SetNavMeshColor(const Color& color);

private:
	/*!*******************************************************************
	\brief
		   Helper method that transforms mouse position to world space.

	\param point
		   Point to transform.

	\return Point
			Point in world space.
	********************************************************************/
	Point ToWorldCoordinate(const Point& point) const;

private:
	const int m_width; //!< Width of search world.
	const int m_height; //!< Height of search world.

	SearchingArea m_area; //!< Object under hierarchy. Handles search area.

	bool m_isMouseDown = false; //!< Indicates whether mouse is currently down or not.
};

#endif // !WORLD_H
