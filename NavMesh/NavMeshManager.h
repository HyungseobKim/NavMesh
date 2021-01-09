/*!*******************************************************************
\headerfile   NavMeshManager.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for NavMeshManager class which has methods 
			  for generating navigation mesh automatically.
********************************************************************/
#ifndef NAVMESHMANAGER_H
#define NAVMESHMANAGER_H

#include <vector>
#include <list>

#include "Point.h"
#include "Color.h"
#include "BinaryTree.h"

class Render;
struct Node;

/*!*******************************************************************
\class NavMeshManager
\brief
	   Has methods execute the algorithms for polygon triangulation. 
********************************************************************/
class NavMeshManager
{
public:
	/*!*******************************************************************
	\struct Vertex
	\brief
		   Basic data sturcture to make doubly-linked list of vertices
		   of polygon.
	********************************************************************/
	struct Vertex
	{
		/*!*******************************************************************
		\struct Type
		\brief
			   Indicates the type of the vertex.

			   Start, end, split, merge, and regular types are used for
			   algorithm that splitting polygon into monotone pieces.

			   Right and left types are used for algorithm that polygon
			   triangulation.
		********************************************************************/
		enum class Type
		{
			Start,
			End,
			Split,
			Merge,
			Regular,
			Right,
			Left,
			Default
		};

		Point m_position; //!< Position of this vertex.
		Type m_type = Type::Default; //!< Type of this vertex.

		Vertex* m_prev = nullptr; //!< Pointer to previous vertex. Which is adjacent vertex in clockwise direction.
		Vertex* m_next = nullptr; //!< Pointer to next vertex. Which is adjacent vertex in counter-clockwise direction.

		bool m_isEdgeToNextAvailable = true; //!< Indicates whether this vertex is used to construct monotone piece.
		std::list<Vertex*> m_diagonals; //!< Container for opposite vertices of diagonal starts from this vertex.
	};

	/*!*******************************************************************
	\struct Edge
	\brief
		   Basic data sturcture indicates the edge of polygon.
		   Will be used for generating navigation mesh algorithms.
	********************************************************************/
	struct Edge
	{
		/*!*******************************************************************
		\brief
			   Constructor that initializes all variables.

		\param start
			   Start point of this edge. Default value is (0, 0).

		\param end
			   End point of this edge. Default value is (0, 0).

		\param helper
			   Pointer to helper vertex of this edge. Default value is nullptr.
		********************************************************************/
		inline Edge(Point start = O, Point end = O, Vertex* helper = nullptr)
			: m_start(start), m_end(end), m_helper(helper) {}

		Point m_start; //!< Start point of this edge.
		Point m_end; //!< End point of this edge.
		Vertex* m_helper; //!< Being used in splitting into monotone pieces algorithm. Indicates which vertex can be used for the next diagonal.

		/*!*******************************************************************
		\brief
			   Overloaded equality operator for this struct.

		\param edge
			   Edge to compare.

		\return bool
			    Returns true if both vertices of edge are same.
				Return false otherwise.
		********************************************************************/
		inline bool operator==(const Edge& edge) const
		{
			return (m_start == edge.m_start) && (m_end == edge.m_end);
		}
	};

	/*!*******************************************************************
	\struct NavMesh
	\brief
		   Basic data sturcture indicates one navigation mesh.
	********************************************************************/
	struct NavMesh
	{
		std::vector<Edge> m_edges; //!< Container for edges of this navmesh.
		std::vector<NavMesh*> m_neighbors; //!< Container for pointer to meshes adjacent to each edge.
		Node* m_node = nullptr; //!< Will be used for A* pathfinding. Pointer to node which attached to this navmesh.
	};

public:
	/*!*******************************************************************
	\brief
		   Setter method for color of the navigation meshes.

	\param color
		   New color.
	********************************************************************/
	void SetColor(const Color& color);

	/*!*******************************************************************
	\brief
		   Draws all navigation meshes.

	\param render
		   Render that has drawing methods.
	********************************************************************/
	void Draw(Render* render) const;

	/*!*******************************************************************
	\brief
		   Kick-off method to generate navigation meshes.
		   Initializes information for algorithms.

	\param vertices
		   Vertices of geography.
		   First four vertices are from boundary box.
		   Others are vertices from holes.
	********************************************************************/
	void Generate(std::vector<Vertex>& vertices);

	/*!*******************************************************************
	\brief
		   Getter method for generated navigation mesh.

	\return std::vector<NavMesh>&
			Array of generated navigation meshes.
	********************************************************************/
	std::vector<NavMesh>& GetNavMeshes();

private:
	/*!*******************************************************************
	\brief
		   Before execute partitioning into monotone pieces algorithm,
		   set types of all vertices.
	********************************************************************/
	void LabelVertices();

	/*!*******************************************************************
	\brief
		   Before triangulating polygon, split polygon into simpler shapes,
		   which is y-monotone piece. It means if you walk along the
		   edges from the highest vertex, you always go down or
		   horizontally, so never go up.
	********************************************************************/
	void PartitionIntoMonotone();

	using BT = BinaryTree<Edge>;
	/*!*******************************************************************
	\brief
		   Helper method for splitting polygon algorithm.
		   Has logic which is way to handle start vertex.

	\param vertex
		   Pointer to vertex to handle. Type must be start.

	\param bt
		   Binary tree which contains all edges intersecting with scan
		   line.
	********************************************************************/
	void HandleStartVertex(Vertex* vertex, BT& bt);
	/*!*******************************************************************
	\brief
		   Helper method for splitting polygon algorithm.
		   Has logic which is way to handle end vertex.

	\param vertex
		   Pointer to vertex to handle. Type must be end.

	\param bt
		   Binary tree which contains all edges intersecting with scan
		   line.
	********************************************************************/
	void HandleEndVertex(Vertex* vertex, BT& bt);
	/*!*******************************************************************
	\brief
		   Helper method for splitting polygon algorithm.
		   Has logic which is way to handle split vertex.

	\param vertex
		   Pointer to vertex to handle. Type must be split.

	\param bt
		   Binary tree which contains all edges intersecting with scan
		   line.
	********************************************************************/
	void HandleSplitVertex(Vertex* vertex, BT& bt);
	/*!*******************************************************************
	\brief
		   Helper method for splitting polygon algorithm.
		   Has logic which is way to handle merge vertex.

	\param vertex
		   Pointer to vertex to handle. Type must be merge.

	\param bt
		   Binary tree which contains all edges intersecting with scan
		   line.
	********************************************************************/
	void HandleMergeVertex(Vertex* vertex, BT& bt);
	/*!*******************************************************************
	\brief
		   Helper method for splitting polygon algorithm.
		   Has logic which is way to handle regular vertex.

	\param vertex
		   Pointer to vertex to handle. Type must be regular.

	\param bt
		   Binary tree which contains all edges intersecting with scan
		   line.
	********************************************************************/
	void HandleRegularVertex(Vertex* vertex, BT& bt);
	/*!*******************************************************************
	\brief
		   Helper method for splitting polygon algorithm.
		   Add new diagnal with given two vertices, and set them as
		   opposite vertex of each.

	\param v1
		   Pointer to one of the vertex of new diagonal.

	\param v2
		   Pointer to one of the vertex of new diagonal.
	********************************************************************/
	void AddDiagonal(Vertex* v1, Vertex* v2);

	/*!*******************************************************************
	\brief
		   After partitioning into monotone algorithm, organizes all
		   edges and added diagonals, so construct each piece.
	********************************************************************/
	void OrganizePieces();
	/*!*******************************************************************
	\brief
		   Helper method for OrganizePiece method.
		   Depends on previous vertex, determine which vertex would be
		   next vertex of current piece.

	\return Vertex*
		    Pointer to next vertex.
	********************************************************************/
	Vertex* GetNext(Vertex& vertex, Vertex* prev);

	/*!*******************************************************************
	\brief
		   Before triangulate polygon, determine each vertex on monotone
		   piece is whether on the left chain or right chain.

		   Walk along the edges from the highest vertex to the lowest
		   vertex in both direction, and set all vertices on left side as
		   left vertex and vertices on right side as right vertex.
	********************************************************************/
	void LabelLeftRight();

	/*!*******************************************************************
	\brief
		   Triangulate each monotone piece.
		   This is greedy algorithm that inserts diagonals as many as
		   possible to the vertices in stack.
	********************************************************************/
	void TriangulatePieces();
	/*!*******************************************************************
	\brief
		   Helper method for polygon triangulation algorithm.
		   Check whether there diagonal can be added between two points.

	\param point
		   Candidate vertex for new diagonal.

	\param curr
		   Current vertex of algorithm loop.

	\param prev
		   The last point succeeded to add diagonal from current vertex.

	\return bool
			Return true, if there could be new diagonal.
			Return false, if it is impossible.
	********************************************************************/
	bool CanDiagonalBeAddedTo(const Point& point, const Vertex& curr, const Point& prev);
	/*!*******************************************************************
	\brief
		   Helper method for polygon triangulation algorithm.
		   Makes navigation mesh with given three points.

	\param p1
		   One of vertices of the nav mesh.

	\param p2
		   One of vertices of the nav mesh.

	\param p3
		   One of vertices of the nav mesh.

	\return NavMesh
			Returns nav mesh constructed by given points.
	********************************************************************/
	NavMesh ConstructNavMesh(const Point& p1, const Point& p2, const Point& p3);

	/*!*******************************************************************
	\brief
		   After polygon triangulation algorithm, set pointers to adjacent
		   nav meshe for each nav mesh.
	********************************************************************/
	void SetNeighborForMesh();

private:
	std::vector<Vertex> m_vertices; //!< Container for vertices from original geography.
	std::list<Edge> m_diagonals; //!< Conatiner for new diagonals added by algorithm.
	std::list<std::vector<Vertex>> m_pieces; //!< Container for monotone pieces. Each piece has the vector of vertices.
	std::vector<NavMesh> m_triangles; //!< Container of generated navigation meshes.
	
	Color m_color = GRAY; //!< Color of navigation meshes.
};

#endif // !NAVMESHMANAGER_H

