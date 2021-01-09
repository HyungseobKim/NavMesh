/*!*******************************************************************
\headerfile   BinaryTree.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Binary Tree class being used for
			  algorithm that split polygon into monotone pieces.
********************************************************************/
#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <vector>

#include "Point.h"

/*!*******************************************************************
\class BinaryHeap
\brief
	   Simple binary searching sorted array.
	   It is templatized, but just worked as container of edges
	   intersecting with scan line.
********************************************************************/
template <class T>
class BinaryTree
{
public:
	/*!*******************************************************************
	\brief
		   Do binary search to find position to insert new edge, so
		   keep array be sorted.

	\param edge
		   New edge to insert. Must not be already in the container.
	********************************************************************/
	void Insert(const T& edge);
	/*!*******************************************************************
	\brief
		   Do binary search to find edge directly left of vertex
		   and returns.

	\param vertex
		   Vertex to check.

	\return T&
			Edge directly left on the vertex in the polygon.
	********************************************************************/
	T& FindDirectlyLeftOf(const Point& vertex);
	/*!*******************************************************************
	\brief
		   Returns corresponding edge to given edge.

	\param edge
		   Edge to find.

	\return T&
			Edge having same vertices with given edge.
	********************************************************************/
	T& FindEdge(const T& edge);
	/*!*******************************************************************
	\brief
		   Delete given edge from array.

	\param edge
		   Edge to delete, which is not interseting with scan line anymore.
	********************************************************************/
	void Delete(const T& edge);

private:
	/*!*******************************************************************
	\brief
		   Helepr function to compute x position which is standard of
		   sorting and finding edges in container.

	\param edge
		   Edge to test.

	\param y
		   Scan line, which is horizontal line.

	\return float
			X-coordinate value of intersection between given edge and y.
	********************************************************************/
	float GetIntersectingX(const T& edge, float y) const;
	/*!*******************************************************************
	\brief
		   Helepr function to find index of given edge in container.

	\param edge
		   Edge to find.
		   
	\return int
			Index of edge in container.
	********************************************************************/
	int FindIndexOfEdge(const T& edge) const;

private:
	std::vector<T> m_edges; //!< Container of edges which always be sorted by increasing x-coordinate order.
};

#include "BinaryTree.hpp"

#endif // !BINARYTREE_H
