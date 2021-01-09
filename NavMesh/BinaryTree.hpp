/*!*******************************************************************
\headerfile   BinaryTree.hpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include "BinaryTree.h"

template <class T>
float BinaryTree<T>::GetIntersectingX(const T& edge, float y) const
{
	const auto& start = edge.m_start;
	const auto& end = edge.m_end;

	const float dx = end.X - start.X;
	const float dy = end.Y - start.Y;
	const float c = dy * start.X - dx * start.Y;

	return (dx * y + c) / dy;
}

template <class T>
void BinaryTree<T>::Insert(const T& edge)
{
	const float x = edge.m_start.X;
	const float y = edge.m_start.Y;

	int min = 0;
	int max = static_cast<int>(m_edges.size());
	int mid = (min + max) / 2;

	while (min != max)
	{
		if (GetIntersectingX(m_edges[mid], y) < x)
		{
			if (min == mid)
				++min;
			else
				min = mid;
		}
		else
			max = mid;

		mid = (min + max) / 2;
	}

	m_edges.insert(m_edges.begin() + mid, edge);
}

template <class T>
T& BinaryTree<T>::FindDirectlyLeftOf(const Point& vertex)
{
	const float x = vertex.X;
	const float y = vertex.Y;

	int min = 0;
	int max = static_cast<int>(m_edges.size());
	int mid = (min + max) / 2;

	while (min != max)
	{
		if (GetIntersectingX(m_edges[mid], y) < x)
		{
			if (min == mid)
				break;
			else
				min = mid;
		}
		else
			max = mid;

		mid = (min + max) / 2;
	}

	return m_edges[mid];
}

template <class T>
int BinaryTree<T>::FindIndexOfEdge(const T& edge) const
{
	const float x = edge.m_end.X;
	const float y = edge.m_end.Y;

	int min = 0;
	int max = static_cast<int>(m_edges.size());
	int mid = (min + max) / 2;

	while (min != max)
	{
		auto& curr = m_edges[mid];

		if (curr == edge)
			break;
		else if (GetIntersectingX(curr, y) < x)
		{
			if (min == mid)
				++min;
			else
				min = mid;
		}
		else
			max = mid;

		mid = (min + max) / 2;
	}

	return mid;
}

template <class T>
T& BinaryTree<T>::FindEdge(const T& edge)
{
	return m_edges[FindIndexOfEdge(edge)];
}

template <class T>
void BinaryTree<T>::Delete(const T& edge)
{
	m_edges.erase(m_edges.begin() + FindIndexOfEdge(edge));
}