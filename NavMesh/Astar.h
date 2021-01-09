/*!*******************************************************************
\headerfile   Astar.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Astar class which has methods and 
			  variables for A* pathfinding.
********************************************************************/
#ifndef ASTAR_H
#define ASTAR_H

#include <vector>

#include "Point.h"

#include "BinaryHeap.h"
#include "NavMeshManager.h"

#include "TriangleMesh.h"
#include "SquareMesh.h"

class Render;

/*!*******************************************************************
\class Astar
\brief
	   Does A* pathfinding with given starting point and end point.
	   There are also variables for that and path smoothing method.
********************************************************************/
class Astar
{
public:
	/*!*******************************************************************
	\brief
		   Constructor that initializes pathfinding points.

	\param start
		   Beginning point of path.

	\param end
		   Destination point of path.
	********************************************************************/
	Astar(const Point& start, const Point& end);

	/*!*******************************************************************
	\brief
		   When geography is changed, get new navigation mesh and
		   initialize all nodes. Then, find path newly.

	\param meshes
		   Navigation meshes for new map.
	********************************************************************/
	void InitializeMap(std::vector<NavMeshManager::NavMesh>& meshes);

	/*!*******************************************************************
	\brief
		   Setter method for start point.
		   Find path newly after setting.

	\param start
		   New start point.
	********************************************************************/
	void SetStart(const Point& start);
	/*!*******************************************************************
	\brief
		   Setter method for end point.
		   Find path newly after setting.

	\param end
		   New end point.
	********************************************************************/
	void SetEnd(const Point& end);
	/*!*******************************************************************
	\brief
		   Setter method for weight.

	\param weight
		   New weight.
	********************************************************************/
	void SetWeight(float weight);
	/*!*******************************************************************
	\brief
		   Setter method for color of considered nodes.

	\param color
		   New color.
	********************************************************************/
	void SetConsideredColor(const Color& color);
	/*!*******************************************************************
	\brief
		   Setter method for color of visited nodes.

	\param color
		   New color.
	********************************************************************/
	void SetVisitedColor(const Color& color);
	/*!*******************************************************************
	\brief
		   Setter method for color of path.

	\param color
		   New color.
	********************************************************************/
	void SetPathColor(const Color& color);

	/*!*******************************************************************
	\brief
		   Draws path from start point to end point.

	\param render
		   Render that has drawing methods.

	\param drawAll
		   If true, draws considered nodes and visited nodes too.
		   Otherwise, draws only path.

	\param smoothPath
		   If true, draws smoothen path.
	********************************************************************/
	void DrawPath(Render* render, bool drawAll, bool smoothPath) const;

private:
	/*!*******************************************************************
	\brief
		   Finding path on the navigation meshes using A* algorithm.
	********************************************************************/
	void FindPath();
	
	// Helper functions for A*
	/*!*******************************************************************
	\brief
		   Find node that include given position.
		   Check bounding box first, and then test for actual polygon.

	\param position
		   Point to check.

	\return Node*
			Pointer to node corresponding to given point.
	********************************************************************/
	Node* GetNode(const Point& position);
	/*!*******************************************************************
	\brief
		   Check whether given position is inside of box bounding given
		   navigation mesh.

	\param navMesh
		   Navigation mesh to check.

	\param position
		   Point to check.

	\return bool
			If point is inside box bounding mesh, returns true.
			Otherwise, return false.
	********************************************************************/
	bool CheckMeshBoundingBox(const NavMeshManager::NavMesh& navMesh, const Point& position) const;
	/*!*******************************************************************
	\brief
		   Check whether given position is inside of given navigation mesh.

	\param navMesh
		   Navigation mesh to check.

	\param position
		   Point to check.

	\return bool
			If point is inside mesh, returns true.
			Otherwise, return false.
	********************************************************************/
	bool IsPointInsideMesh(const NavMeshManager::NavMesh& navMesh, const Point& position) const;

	/*!*******************************************************************
	\brief
		   Compute heuristic from given position to destination.
		   Used octile method.

	\param position
		   Point to compute.

	\return float
			Return computed heuristic.
	********************************************************************/
	float ComputeHeuristic(const Point& position) const;

	// Methods for UI
	/*!*******************************************************************
	\brief
		   Move box to the start point.

	\param position
		   Position to move which is start point of path.
	********************************************************************/
	void MoveSquare(const Point& position);
	/*!*******************************************************************
	\brief
		   Move triangle to the end point.

	\param position
		   Position to move which is end point of path.
	********************************************************************/
	void MoveTriangle(const Point& position);

	// Methods for path smoothing
	/*!*******************************************************************
	\brief
		   Smooth path using simple stupid funnel algorithm.
	********************************************************************/
	void FunnelAlgorithm();
	/*!*******************************************************************
	\brief
		   Determine left and right vertex of edge of given mesh.

	\param index
		   Index of mesh to check.

	\return std::pair<Point, Point>
			First is left vertex of edge,
			while second is right vertex of edge.
	********************************************************************/
	std::pair<Point, Point> GetLeftRightOfNextEdge(int index);
	/*!*******************************************************************
	\brief
		   Determine left and right vertex of given edge upon given point.

	\param edge
		   Edge to check.

	\param origin
		   Standard point.

	\return std::pair<Point, Point>
			First is left vertex of edge,
			while second is right vertex of edge.
	********************************************************************/
	std::pair<Point, Point> LeftRightPointsOfEdge(const NavMeshManager::Edge& edge, const Point& origin) const;

private:
	std::vector<NavMeshManager::NavMesh> m_meshes; //!< Container of navigation meshes.
	std::vector<Node> m_map; //!< Container of nodes for A* pathfinding.

	int m_iteration = -1; //!< Indicates how many times of pathfinding proceeded on the current map. Reset nodes if and only if nodes are old, so do not need to clear all nodes everytime.
	bool m_pathExists = false; //!< True if there is a valid path between current start point and end point.
	float m_weight = 1.f; //!< Weight for heuristic. If it is greater than 1, it A* does not guarantee optimal path, but searching speed would be faster. If it is 0, it is as same as dijkstra.

	std::vector<Node*> m_path; //!< Container of nodes of final path.
	std::vector<Point> m_smoothPath; //!< Container of points on the smoothen path.
	std::vector<std::pair<Point, Point>> m_visited; //!< Container of all visited nodes. Just for showing.
	std::vector<std::pair<Point, Point>> m_considered; //!< Container of all nodes pushed on open list. Just for showing.

	Point m_start; //!< Beginning point of path.
	Point m_end; //!< Destination point of path.

	Node* m_startNode = nullptr; //!< The node which start point is included.
	Node* m_endNode = nullptr; //!< The node which end point is included.

	SquareMesh m_square; //!< UI box for showing start point.
	TriangleMesh m_triangle; //!< UI triangle for showing end point.

	Color m_cConsidered = BLUE; //!< Color of all nodes pushed into open list.
	Color m_cVisited = ORANGE; //!< Color of all visited node.
	Color m_cPath = RED; //!< Color of path.
};

#endif // !ASTAR_H
