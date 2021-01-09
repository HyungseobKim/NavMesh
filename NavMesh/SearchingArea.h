/*!*******************************************************************
\headerfile   SearchingArea.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for SearchingArea class which defines and
			  manages the search area.
********************************************************************/
#ifndef SEARCHINGAREA_H
#define SEARCHINGAREA_H

#include <vector>

#include "Point.h"
#include "Hole.h"
#include "NavMeshManager.h"
#include "Astar.h"

class Render;

/*!*******************************************************************
\class SearchingArea
\brief
	   Having methods to draw boundary of search area and manages
	   underlying objects.
********************************************************************/
class SearchingArea
{
public:
	/*!*******************************************************************
	\brief
		   Constructor that initializes the size of area and two vertices
		   of path.

	\param width
		   Width of area.

	\param height
		   Height of area.

	\param start
		   Beginning point of path.

	\param end
		   Destination point of path.
	********************************************************************/
	SearchingArea(float width, float height, const Point& start, const Point& end);

	/*!*******************************************************************
	\brief
		   Calls update method for all holes in this area and stores the
		   index of the holes that is picked by user.

	\param mouse
		   Poisition of mouse in world space.
	********************************************************************/
	void Update(const Point& mouse);
	/*!*******************************************************************
	\brief
		   Draws boundary and holes.
		   Draws nav meshes and path, if it is not edit mode.

	\param render
		   Render that has drawing methods.

	\param editMode
		   If it is false, draw nav meshes and path.

	\param showAllNodes
		   Passing value to the underlying object.

	\param smoothPath
		   Passing value to the underlying object.
	********************************************************************/
	void Draw(Render* render, bool editMode, bool showAllNodes, bool smoothPath) const;

	/*!*******************************************************************
	\brief
		   Inserts new hole.
	********************************************************************/
	void AddHole();
	/*!*******************************************************************
	\brief
		   Remove picked hole.
	********************************************************************/
	void RemoveHole();
	/*!*******************************************************************
	\brief
		   Move picked hole to the given position.

	\param position
		   Position to move hole.
	********************************************************************/
	void MoveHole(const Point& position);

	/*!*******************************************************************
	\brief
		   Called when map has been changed.
		   Gathers all vertices from boundary and holes, and gives to
		   nav mesh manager. After that, give nav meshes to astar
		   pathfinder.
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
	const float m_width; //!< Width of search area.
	const float m_height; //!< Height of search area.

	const Point m_boundary[4]; //!< Array of points from boundary box.

	int m_picked = -1; //!< Index of hole which is currently picked by user.
	std::vector<Hole> m_holes; //!< Container of holes in this area.

	NavMeshManager m_navmesh; //!< Object under hierarchy. Handles navigation meshes.
	Astar m_astar; //!< Object under hierarchy. A* pathfinder.
};

#endif // !SEARCHINGAREA_H
