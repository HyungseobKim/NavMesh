/*!*******************************************************************
\file		  NavMeshManager.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include <queue>
#include <stack>
#include <algorithm>

#include "NavMeshManager.h"

#include "Render.h"

void NavMeshManager::Generate(std::vector<Vertex>& vertices)
{
	m_vertices = std::move(vertices);

	LabelVertices();

	PartitionIntoMonotone();

	OrganizePieces();

	LabelLeftRight();

	TriangulatePieces();

	SetNeighborForMesh();
}

void NavMeshManager::SetColor(const Color& color)
{
	m_color = color;
}

void NavMeshManager::Draw(Render* render) const
{
	// There is no duplicated drawing.

	for (auto& edge : m_diagonals)
		render->DrawLine(edge.m_start, edge.m_end, m_color);

	for (auto& vertex : m_vertices)
		render->DrawLine(vertex.m_position, vertex.m_next->m_position, BLACK);
}

std::vector<NavMeshManager::NavMesh>& NavMeshManager::GetNavMeshes()
{
	return m_triangles;
}

/*!*******************************************************************
\struct Compare
\brief
		Overloading operator to compare inside priority_queue.
********************************************************************/
struct Compare
{
	/*!*******************************************************************
	\brief
			Overloaded parenthesis operator to compare two vertex pointers.

	\param v1
		   Pointer to one of vertices.

	\param v2
		   Pointer to one of vertices.

	\return bool
			Returns true if height of v1 is lower than v2.
			If heights are same, then returns true if v1 is on left of v2.
			Otherwise, returns false.
	********************************************************************/
	bool operator()(const NavMeshManager::Vertex* v1, const NavMeshManager::Vertex* v2)
	{
		const auto& pos1 = v1->m_position;
		const auto& pos2 = v2->m_position;

		// Compare y first. If same, compare x too.
		// Both vertices are from one same polygon, so they can not be same.
		return (pos1.Y < pos2.Y) || ((pos1.Y == pos2.Y) && (pos1.X > pos2.X));
	}
};

void NavMeshManager::LabelVertices()
{
	Compare compare;

	// Boundary
	for (int i = 0; i < 4; ++i)
	{
		auto& vertex = m_vertices[i];
		const auto& prev = vertex.m_prev->m_position;
		const auto& next = vertex.m_next->m_position;
		
		// If both neighbor vertices are "lower" than this vertex, then type is Start.
		if (compare(vertex.m_prev, &vertex) && compare(vertex.m_next, &vertex))
			vertex.m_type = Vertex::Type::Start;
		// If this vertex is "lower" than both neighbor vertices, then type is End.
		else if (compare(&vertex, vertex.m_prev) && compare(&vertex, vertex.m_next))
			vertex.m_type = Vertex::Type::End;
		else // there is no Split or Merge vertex on the boundary box.
			vertex.m_type = Vertex::Type::Regular;
	}

	// Holes
	const size_t size = m_vertices.size();
	for (size_t i = 4; i < size; ++i)
	{
		auto& vertex = m_vertices[i];
		const auto& prev = vertex.m_prev->m_position;
		const auto& next = vertex.m_next->m_position;

		// If both neighbor vertices are "lower" than this vertex, then type is Split.
		if (compare(vertex.m_prev, &vertex) && compare(vertex.m_next, &vertex))
			vertex.m_type = Vertex::Type::Split;
		// If this vertex is "lower" than both neighbor vertices, then type is Merge.
		else if (compare(&vertex, vertex.m_prev) && compare(&vertex, vertex.m_next))
			vertex.m_type = Vertex::Type::Merge;
		else // there is no Start or End vertex on the hole.
			vertex.m_type = Vertex::Type::Regular;
	}
}

void NavMeshManager::PartitionIntoMonotone()
{
	m_diagonals = std::list<Edge>();
	std::priority_queue<Vertex*, std::vector<Vertex*>, Compare> queue;

	// Put all vertices into priority queue
	for (auto& vertex : m_vertices)
		queue.push(&vertex);

	BT bt;

	// Handle all vertices by height. Think like scanning entire polygon with horizontal line.
	while (!queue.empty())
	{
		Vertex* vertex = queue.top();
		queue.pop();

		switch (vertex->m_type)
		{
		case Vertex::Type::Start:
			HandleStartVertex(vertex, bt);
			break;

		case Vertex::Type::End:
			HandleEndVertex(vertex, bt);
			break;

		case Vertex::Type::Split:
			HandleSplitVertex(vertex, bt);
			break;

		case Vertex::Type::Merge:
			HandleMergeVertex(vertex, bt);
			break;

		case Vertex::Type::Regular:
			HandleRegularVertex(vertex, bt);
			break;
		}
	}
}

void NavMeshManager::HandleStartVertex(Vertex* vertex, BT& bt)
{
	// Since this is start vertex, we can know this edge will start to intersect with scan line from here.
	Edge ei(vertex->m_position, vertex->m_next->m_position, vertex);
	bt.Insert(ei);
}

void NavMeshManager::HandleEndVertex(Vertex* vertex, BT& bt)
{
	// Find edge of previous vertex
	auto& ei_1 = bt.FindEdge(Edge(vertex->m_prev->m_position, vertex->m_position));
	
	// If helper of edge is merge vertex, add diagonal
	if (ei_1.m_helper->m_type == Vertex::Type::Merge)
		AddDiagonal(vertex, ei_1.m_helper);

	// Since this is end vertex, we can know previous edge will not intersect with scan line anymore.
	bt.Delete(ei_1);
}

void NavMeshManager::HandleSplitVertex(Vertex* vertex, BT& bt)
{
	// Find directly left edge of this vertex.
	auto& ej = bt.FindDirectlyLeftOf(vertex->m_position);

	// Since inner angle of split vertex is greater than 180, this is good candidate to add diagonal.
	// More precisely, it always can have new diagonal.
	AddDiagonal(vertex, ej.m_helper);

	// Helper of this edge is already used, so change it.
	ej.m_helper = vertex;

	// By definition of split vertex, we can know this edge will start to intersect with scan line from here.
	Edge ei(vertex->m_position, vertex->m_next->m_position, vertex);
	bt.Insert(ei);
}

void NavMeshManager::HandleMergeVertex(Vertex* vertex, BT& bt)
{
	// Find edge of previous vertex
	auto& ei_1 = bt.FindEdge(Edge(vertex->m_prev->m_position, vertex->m_position));
	if (ei_1.m_helper->m_type == Vertex::Type::Merge)
		AddDiagonal(vertex, ei_1.m_helper);

	// By definition of merge vertex, we can know previous edge will not intersect with scan line anymore.
	bt.Delete(ei_1);

	// Find directly left edge of this vertex.
	auto& ej = bt.FindDirectlyLeftOf(vertex->m_position);
	if (ej.m_helper->m_type == Vertex::Type::Merge)
		AddDiagonal(vertex, ej.m_helper);

	// Since inner angle of merge vertex is greater than 180, this is good candidate to add diagonal.
	// Threfore, set helper as this vertex.
	ej.m_helper = vertex;
}

void NavMeshManager::HandleRegularVertex(Vertex* vertex, BT& bt)
{
	const float currY = vertex->m_position.Y;
	const float nextY = vertex->m_next->m_position.Y;

	// If interior of polygon lies to the right of vertex,
	if (nextY < currY || (nextY == currY && vertex->m_prev->m_position.Y > currY))
	{
		// Find edge of previous vertex
		auto& ei_1 = bt.FindEdge(Edge(vertex->m_prev->m_position, vertex->m_position));
		if (ei_1.m_helper->m_type == Vertex::Type::Merge)
			AddDiagonal(vertex, ei_1.m_helper);

		// By definition of regular vertex, we can know previous edge will not intersect with scan line anymore.
		bt.Delete(ei_1);

		// By definition of regular vertex, we can know this edge will start to intersect with scan line from here.
		Edge ei(vertex->m_position, vertex->m_next->m_position, vertex);
		bt.Insert(ei);
	}
	else
	{
		// Find directly left edge of this vertex.
		auto& ej = bt.FindDirectlyLeftOf(vertex->m_position);
		if (ej.m_helper->m_type == Vertex::Type::Merge)
			AddDiagonal(vertex, ej.m_helper);

		// Set this vertex as next candidate.
		ej.m_helper = vertex;
	}
}

void NavMeshManager::AddDiagonal(Vertex* v1, Vertex* v2)
{
	// Set each vertex as opposite vertex of them.
	v1->m_diagonals.push_back(v2);
	v2->m_diagonals.push_back(v1);

	m_diagonals.push_back(Edge(v1->m_position, v2->m_position));
}

void NavMeshManager::OrganizePieces()
{
	m_pieces = std::list<std::vector<Vertex>>();

	// Looking all vertices
	for (auto& vertex : m_vertices)
	{
		// If edge of this vertex is already used as part of piece, continue.
		// We can gaurantee that all edges from primitive polygon will be part of only one piece.
		if (vertex.m_isEdgeToNextAvailable == false) continue;

		// Make new monotone piece start from this vertex
		m_pieces.push_back(std::vector<Vertex>());
		auto& piece = *(--m_pieces.end());

		piece.push_back(vertex);
		vertex.m_isEdgeToNextAvailable = false;

		Vertex* prev = &vertex;
		Vertex* curr = vertex.m_next;

		// Add edges until we can make closed shape
		while (curr != &vertex)
		{
			piece.push_back(*curr);
			
			// Get next edge of this piece from current vertex
			Vertex* next = GetNext(*curr, prev);
			if (next == curr->m_next) // If selected edge is not diagonal, mark it as used.
				curr->m_isEdgeToNextAvailable = false;

			prev = curr;
			curr = next;
		}
	}
}

NavMeshManager::Vertex* NavMeshManager::GetNext(Vertex& vertex, Vertex* prev)
{
	Vertex* next = vertex.m_next;

	// Find diagonal has the smallest angle
	const Vector standard = prev->m_position - vertex.m_position;
	float smallest = GetAngleBetween(standard, next->m_position - vertex.m_position);

	for (auto& diagonal : vertex.m_diagonals)
	{
		if (diagonal == prev) continue;

		const Vector diagonal_vector = diagonal->m_position - vertex.m_position;
		const float angle = GetAngleBetween(standard, diagonal_vector);

		if (angle < smallest)
		{
			next = diagonal;
			smallest = angle;
		}
	}
	
	// Edge has the smallest angle to innder direction will be the next.
	return next;
}

void NavMeshManager::LabelLeftRight()
{
	// For each piece
	for (auto& piece : m_pieces)
	{
		const int size = static_cast<int>(piece.size());

		// Find higest and lowest vertices of piece
		int highest = 0;
		float high_value = piece[highest].m_position.Y;

		int lowest = 1;
		float low_value = piece[lowest].m_position.Y;

		if (high_value < low_value || (high_value == low_value && piece[highest].m_position.X > piece[lowest].m_position.X))
		{
			std::swap(highest, lowest);
			std::swap(high_value, low_value);
		}

		for (int i = 2; i < size; ++i)
		{
			const float height = piece[i].m_position.Y;

			if (height > high_value)
			{
				high_value = height;
				highest = i;
			}
			else if (height == high_value)
			{
				if (piece[i].m_position.X < piece[highest].m_position.X)
				{
					high_value = height;
					highest = i;
				}
			}
			else if (height < low_value)
			{
				low_value = height;
				lowest = i;
			}
			else if (height == low_value)
			{
				if (piece[i].m_position.X > piece[lowest].m_position.X)
				{
					low_value = height;
					lowest = i;
				}
			}
		}

		// Label vertices
		for (int i = highest; i != lowest; i = (i + 1) % size)
			piece[i].m_type = Vertex::Type::Left;

		for (int i = lowest; i != highest; i = (i + 1) % size)
			piece[i].m_type = Vertex::Type::Right;

		piece[highest].m_type = piece[lowest].m_type = Vertex::Type::Default;

		// Sort vertices by height for triangulation algorithm.
		std::sort(piece.begin(), piece.end(), [](const auto& lhs, const auto& rhs) {
			const auto& posL = lhs.m_position;
			const auto& posR = rhs.m_position;
			
			return (posL.Y > posR.Y) || (posL.Y == posR.Y && posL.X < posR.X);
		});
	}
}

void NavMeshManager::TriangulatePieces()
{
	m_triangles = std::vector<NavMesh>();

	// For each piece
	for (auto& piece : m_pieces)
	{
		std::stack<Vertex*> stack;

		stack.push(&piece[0]);
		stack.push(&piece[1]);

		const int last = static_cast<int>(piece.size() - 1);
		for (int i = 2; i < last; ++i)
		{
			Vertex& curr = piece[i];
			Vertex* top = stack.top();

			// If they are on different chain
			if (curr.m_type != top->m_type)
			{
				// Add diagonal to all vertices in stack except last one,
				// because last vertex is already connected to current vertex
				Vertex* next = stack.top();
				while (stack.size() > 1)
				{
					top = next;
					stack.pop();

					Edge edge(curr.m_position, top->m_position);
					m_diagonals.push_back(edge);
					
					next = stack.top();
					m_triangles.push_back(std::move(ConstructNavMesh(edge.m_start, edge.m_end, next->m_position)));
				}
				stack.pop();

				// Push last two vertices
				stack.push(&piece[i - 1]);
				stack.push(&curr);
			}
			else // on same chain
			{
				// Vertex at the top is already connected to current vertex, so pop this one.
				stack.pop();
				Vertex* prev = top;
				
				// When stack is empty, stops.
				while (!stack.empty())
				{
					top = stack.top();

					// If each vertex can be used to add diagonal, add new one.
					if (CanDiagonalBeAddedTo(top->m_position, curr, prev->m_position))
					{
						Edge edge(curr.m_position, top->m_position);
						m_diagonals.push_back(edge);

						m_triangles.push_back(std::move(ConstructNavMesh(curr.m_position, top->m_position, prev->m_position)));

						prev = top;
						stack.pop();
					}
					else // If there is a vertex cannot be connected to current vertex, rest of stack also impossible, so stops here.
						break;
				}

				// Push vertex that popped lastly and current vertex.
				stack.push(prev);
				stack.push(&curr);
			}
		}

		const Point& last_pos = piece[last].m_position;
		
		Vertex* prev = stack.top();
		stack.pop();

		Vertex* top;

		// From the last vertex, add diagonal to the all rest of vertices,
		// except last one which is already connected to last vertex.
		while (stack.size() > 1)
		{
			top = stack.top();
			stack.pop();

			Edge edge(last_pos, top->m_position);
			m_diagonals.push_back(edge);

			m_triangles.push_back(std::move(ConstructNavMesh(last_pos, top->m_position, prev->m_position)));

			prev = top;
		}

		top = stack.top();

		// Construct last Nav mesh triangle of this piece
		m_triangles.push_back(std::move(ConstructNavMesh(last_pos, top->m_position, prev->m_position)));
	}
}

bool NavMeshManager::CanDiagonalBeAddedTo(const Point& point, const Vertex& curr, const Point& prev)
{
	// If point is over the line passing through current vertex and previous point, diagonal cannot be added.

	const auto& pos = curr.m_position;
	const float angle = GetAngleBetween(prev - pos, point - pos);

	if (curr.m_type == Vertex::Type::Left)
		return angle < 180.f;

	return angle > 180.f;
}

NavMeshManager::NavMesh NavMeshManager::ConstructNavMesh(const Point& p1, const Point& p2, const Point& p3)
{
	NavMesh mesh;

	mesh.m_edges.push_back(Edge(p1, p2));
	mesh.m_edges.push_back(Edge(p2, p3));
	mesh.m_edges.push_back(Edge(p3, p1));
	mesh.m_neighbors.resize(3, nullptr);

	return mesh;
}

void NavMeshManager::SetNeighborForMesh()
{
	// For each diagonal,
	for (auto& diagonal : m_diagonals)
	{
		NavMesh* first = nullptr;
		int index = -1;

		// find the two meshes that sharing this diagonal.
		for (auto& mesh : m_triangles)
		{
			auto& edges = mesh.m_edges;
			const size_t size = edges.size();

			for (size_t i = 0; i < size; ++i)
			{
				auto& edge = edges[i];

				if (edge == diagonal || // Check reversed case too.
					(edge.m_start == diagonal.m_end && edge.m_end == diagonal.m_start))
				{
					// If this is first one, store and find second one.
					if (first == nullptr)
					{
						first = &mesh;
						index = i;
						break;
					}
					else // If this second one, set each nav mesh as neighbor of them.
					{
						mesh.m_neighbors[i] = first;
						first->m_neighbors[index] = &mesh;
						goto Continue; // We do not need to care this diagonal anymore.
					}
				}
			}
		}

	Continue:;
	}
}