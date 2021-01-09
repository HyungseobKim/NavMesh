/*!*******************************************************************
\file		  Astar.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include <limits>
#include <algorithm>
#include <cmath>

#include "Astar.h"

#include "Render.h"

Astar::Astar(const Point& start, const Point& end)
	: m_start(start), m_end(end)
{
	// Initialize node for each point.
	m_startNode = GetNode(m_start);
	m_endNode = GetNode(m_end);

	// Initialize size and color of box.
	// Put on the start point.
	m_square.SetVertex(0, Point(-5.f, -5.f));
	m_square.SetVertex(1, Point(5.f, -5.f));
	m_square.SetVertex(2, Point(5.f, 5.f));
	m_square.SetVertex(3, Point(-5.f, 5.f));
	m_square.SetColor(RED);
	MoveSquare(start);

	// Initialize size and color of triangle.
	// Put on the end point.
	m_triangle.SetVertex(0, Point(0.f, 7.f));
	m_triangle.SetVertex(1, Point(7.f, -7.f));
	m_triangle.SetVertex(2, Point(-7.f, -7.f));
	m_triangle.SetColor(RED);
	MoveTriangle(end);
}

void Astar::InitializeMap(std::vector<NavMeshManager::NavMesh>& meshes)
{
	m_meshes = std::move(meshes);
	
	const size_t size = m_meshes.size();
	m_map.resize(size);

	// Initialize each node for each mesh.
	for (int i = 0; i < size; ++i)
	{
		auto& node = m_map[i];
		node.m_navMesh = &m_meshes[i];
		m_meshes[i].m_node = &node;
		node.m_iteration = -1;
	}

	// Initialize variables for pathfinding newly.
	m_startNode = GetNode(m_start);
	m_endNode = GetNode(m_end);
	m_iteration = 0;

	// Find path newly.
	FindPath();
}

void Astar::SetStart(const Point& start)
{
	// Find start node newly.
	m_start = start;
	m_startNode = GetNode(m_start);

	// Find path newly and move box to new start point.
	FindPath();
	MoveSquare(m_start);
}

void Astar::SetEnd(const Point& end)
{
	// Find end node newly.
	m_end = end;
	m_endNode = GetNode(m_end);

	// Find path newly and move triangle to new end point.
	FindPath();
	MoveTriangle(m_end);
}

void Astar::SetWeight(float weight)
{
	m_weight = weight;
}

void Astar::SetConsideredColor(const Color& color)
{
	m_cConsidered = color;
}

void Astar::SetVisitedColor(const Color& color)
{
	m_cVisited = color;
}

void Astar::SetPathColor(const Color& color)
{
	m_cPath = color;

	m_square.SetColor(color);
	m_triangle.SetColor(color);
}

void Astar::DrawPath(Render* render, bool drawAll, bool smoothPath) const
{
	// Draw all edges from nodes pushed to open list.
	if (drawAll)
	{
		for (auto& edge : m_considered)
			render->DrawLine(edge.first, edge.second, m_cConsidered);

		for (auto& edge : m_visited)
			render->DrawLine(edge.first, edge.second, m_cVisited);
	}

	// Draw Path
	if (m_pathExists)
	{
		Point prev = m_start;

		if (smoothPath)
		{
			for (auto& point : m_smoothPath)
			{
				render->DrawLine(prev, point, m_cPath, 2.f);
				prev = point;
			}

			render->DrawLine(prev, m_end, m_cPath, 2.f);
		}
		else
		{
			for (auto& node : m_path)
			{
				Point curr = node->m_origin;
				render->DrawLine(prev, curr, m_cPath, 2.f);

				prev = curr;
			}
		}
	}
	
	// Draw shapes on start & end point
	render->DrawMesh(m_triangle);
	render->DrawMesh(m_square);
}

void Astar::FindPath()
{
	// Clear all contianers and reset variables.
	m_pathExists = true;
	m_path = std::vector<Node*>();
	m_visited = std::vector<std::pair<Point, Point>>();
	m_considered = std::vector<std::pair<Point, Point>>();
	
	// Special case. One of vertices is not inside of polygon.
	if (m_startNode == nullptr || m_endNode == nullptr)
	{
		m_pathExists = false;
		return;
	}

	++m_iteration;
	BinaryHeap openList;

	// Set up first point. Start from end point, so easily can restore path.
	m_endNode->m_iteration = m_iteration;
	m_endNode->m_origin = m_end;

	m_endNode->m_cost = ComputeHeuristic(m_end);
	m_endNode->m_given = 0.f;

	m_endNode->m_parent = nullptr;
	m_endNode->m_status = Node::Status::Open;

	openList.Insert(m_endNode);

	// During there nodes remain inside open list, keep looking for path.
	while (!openList.Empty())
	{
		// Grab node has lowest cost.
		Node* thisNode = openList.GetTop();
		thisNode->m_status = Node::Status::Closed;

		if (thisNode->m_parent)
			m_visited.push_back(std::make_pair(thisNode->m_origin, thisNode->m_parent->m_origin));

		// Path found
		if (m_startNode == thisNode)
		{
			while (thisNode)
			{
				m_path.push_back(thisNode);
				thisNode = thisNode->m_parent;
			}

			FunnelAlgorithm();
			return;
		}

		auto navMesh = thisNode->m_navMesh;
		const int size = static_cast<int>(navMesh->m_edges.size());

		// Look all neighbors of this navigation mesh.
		for (int i = 0; i < size; ++i)
		{
			if (navMesh->m_neighbors[i] == nullptr) continue;

			Node* currNode = navMesh->m_neighbors[i]->m_node;
			const auto& edge = navMesh->m_edges[i];

			// If this node has never been visitid for current pathfinding
			if (currNode->m_iteration != m_iteration ||
				(currNode->m_iteration == m_iteration && currNode->m_status == Node::Status::Default))
			{
				currNode->m_iteration = m_iteration;
				currNode->m_origin = (edge.m_start + edge.m_end) / 2.f; // Middle point of edge which is sharing. Standard for pathfinding.

				// Given cost is distance between two points + given cost of parent node.
				currNode->m_given = thisNode->m_given + DistanceBetween(currNode->m_origin, thisNode->m_origin);
				currNode->m_cost = ComputeHeuristic(currNode->m_origin) + currNode->m_given;

				currNode->m_parent = thisNode;
				currNode->m_status = Node::Status::Open;

				m_considered.push_back(std::make_pair(currNode->m_origin, thisNode->m_origin));
				openList.Insert(currNode);
			}
			// This node is already inside open list.
			else if (currNode->m_iteration == m_iteration && currNode->m_status == Node::Status::Open)
			{
				Point new_origin = (edge.m_start + edge.m_end) / 2.f;

				float new_given = thisNode->m_given + DistanceBetween(new_origin, thisNode->m_origin);
				float new_cost = new_given + ComputeHeuristic(new_origin);

				// If new cost is lower than existing cost,
				if (new_cost < currNode->m_cost)
				{
					// update the node.
					openList.DecreaseKey(currNode, new_cost);

					currNode->m_origin = new_origin;
					currNode->m_parent = thisNode;

					currNode->m_given = new_given;
					currNode->m_cost = new_cost;

					m_considered.push_back(std::make_pair(currNode->m_origin, thisNode->m_origin));
				}
			}
		}
	}
}

Node* Astar::GetNode(const Point& position)
{
	for (auto& mesh : m_meshes)
	{
		// Check cheap test first. Box bounding mesh.
		if (CheckMeshBoundingBox(mesh, position) == false) continue;

		// Check expensive test.
		if (IsPointInsideMesh(mesh, position))
			return mesh.m_node;
	}

	return nullptr;
}

bool Astar::CheckMeshBoundingBox(const NavMeshManager::NavMesh& navMesh, const Point& position) const
{
	const auto& minmaxX = std::minmax_element(navMesh.m_edges.begin(), navMesh.m_edges.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.m_start.X < rhs.m_start.X;
	});

	if (position.X < (*(minmaxX.first)).m_start.X)
		return false;
	if (position.X > (*(minmaxX.second)).m_start.X)
		return false;

	const auto& minmaxY = std::minmax_element(navMesh.m_edges.begin(), navMesh.m_edges.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.m_start.Y < rhs.m_start.Y;
	});

	if (position.Y < (*(minmaxY.first)).m_start.Y)
		return false;
	if (position.Y > (*(minmaxY.second)).m_start.Y)
		return false;

	return true;
}

bool Astar::IsPointInsideMesh(const NavMeshManager::NavMesh& navMesh, const Point& position) const
{
	const auto& edges = navMesh.m_edges;

	// Get clockwise angle from edge vector to point vector.
	float angle = GetAngleBetween(edges[0].m_end - edges[0].m_start, position - edges[0].m_start);
	const bool direction = (angle <= 180.f); // Position of point upon first edge.

	const size_t size = edges.size();
	for (size_t i = 1; i < size; ++i)
	{
		auto& edge = edges[i];
		angle = GetAngleBetween(edge.m_end - edge.m_start, position - edge.m_start);

		// Direction must be same for all edges.
		if ((angle <= 180.f) != direction)
			return false;
	}

	return true;
}

float Astar::ComputeHeuristic(const Point& position) const
{
	const float dx = std::abs(m_start.X - position.X);
	const float dy = std::abs(m_start.Y - position.Y);

	const float shorter = std::min(dx, dy);
	return (shorter * std::sqrtf(2.f) + std::max(dx, dy) - shorter) * m_weight;
}

void Astar::MoveTriangle(const Point& position)
{
	const Vector delta = position - m_triangle.GetCenter();

	const auto& vertices = m_triangle.GetVertices();
	for (int i = 0; i < 3; ++i)
		m_triangle.SetVertex(i, vertices[i] + delta);
}

void Astar::MoveSquare(const Point& position)
{
	const Vector delta = position - m_square.GetCenter();

	const auto& vertices = m_square.GetVertices();
	for (int i = 0; i < 4; ++i)
		m_square.SetVertex(i, vertices[i] + delta);
}

void Astar::FunnelAlgorithm()
{
	// Reset container.
	m_smoothPath = std::vector<Point>();

	// If there are less than 2 nodes, it means path is a single straight line already.
	const int size = static_cast<int>(m_path.size());
	if (size < 2) return;
	
	// Construct left and right vertices array
	std::vector<Point> left_vertices(size);
	std::vector<Point> right_vertices(size);

	// Fill left and right vertices of funnel
	for (int i = 0; i < size - 1; ++i)
	{
		auto points = GetLeftRightOfNextEdge(i);

		left_vertices[i] = points.first;
		right_vertices[i] = points.second;
	}

	// Last vertex of both sides is destination point.
	left_vertices[size - 1] = m_end;
	right_vertices[size - 1] = m_end;

	// Variables for loop
	int leftIndex = 0;
	int rightIndex = 0;

	// Store as pointer, so it is cheaper and easy to compare.
	Point* corner = &m_start;
	Point* portalLeft = &left_vertices[leftIndex];
	Point* portalRight = &right_vertices[rightIndex];

	for (int i = 1; i < size; ++i)
	{
		auto& left = left_vertices[i];
		auto& right = right_vertices[i];

		// Update right vertex of edge
		if (right != *portalRight)
		{
			// If new vertex can tighten the funnel,
			if (TriArea(*corner, *portalRight, right) <= 0.f)
			{
				// portalRight just needs to be updated OR new vertex does not go over opposite side
				if (corner == portalRight || TriArea(*corner, *portalLeft, right) > 0.f)
				{
					portalRight = &right; // Tighten the funnel.
					rightIndex = i;
				}
				else // Right over left.
				{
					// Left becomes new corner and added to path.
					corner = portalLeft;
					m_smoothPath.push_back(*corner);

					// Start from new corner
					i = leftIndex;

					// Set new left and right of portal
					++leftIndex;
					rightIndex = leftIndex;

					if (leftIndex >= size) return;

					portalLeft = &left_vertices[leftIndex];
					portalRight = &right_vertices[rightIndex];

					continue; // Not update left this time
				}
			}
		}

		// Update left vertex
		if (left != *portalLeft)
		{
			// If new vertex can tighten the funnel,
			if (TriArea(*corner, *portalLeft, left) >= 0.f)
			{
				// portalLeft just needs to be updated OR new vertex does not go over opposite side
				if (corner == portalLeft || TriArea(*corner, *portalRight, left) < 0.f)
				{
					portalLeft = &left; // Tighten the funnel.
					leftIndex = i;
				}
				else // Left over right.
				{
					// Right becomes new corner and added to path.
					corner = portalRight;
					m_smoothPath.push_back(*corner);

					// Start from new corner
					i = rightIndex;

					// Set new left and right of portal
					++rightIndex;
					leftIndex = rightIndex;

					if (leftIndex >= size) return;

					portalLeft = &left_vertices[leftIndex];
					portalRight = &right_vertices[rightIndex];
				}
			}
		}
	}
}

std::pair<Point, Point> Astar::GetLeftRightOfNextEdge(int index)
{
	// Store initial index
	const int initial = index;

	Node* curr = m_path[index];
	Node* next = m_path[index + 1];

	// Find sharing edge
	index = 0;
	while (curr->m_navMesh->m_neighbors[index] != next->m_navMesh)
		++index;

	auto& edge = curr->m_navMesh->m_edges[index];

	if (initial == 0)
		return LeftRightPointsOfEdge(edge, m_start);

	return LeftRightPointsOfEdge(edge, m_path[initial - 1]->m_origin);
}

std::pair<Point, Point> Astar::LeftRightPointsOfEdge(const NavMeshManager::Edge& edge, const Point& origin) const
{
	std::pair<Point, Point> result;

	result.first = edge.m_start;
	result.second = edge.m_end;

	// Check one of vertex of edge depends on the line connects given point and middle point of edge
	if (GetAngleBetween((edge.m_start + edge.m_end) / 2.f - origin, edge.m_end - origin) > 180.f)
		std::swap(result.first, result.second);

	return result;
}