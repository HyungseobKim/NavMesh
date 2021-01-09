/*!*******************************************************************
\headerfile   BinaryHeap.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Binary Heap class being used as
			  open list for A* algorithm.
********************************************************************/
#ifndef BINARY_HEAP
#define BINARY_HEAP

#include <vector>

#include "NavMeshManager.h"

/*!*******************************************************************
\struct Node
\brief
	   Base data structure for A* pathfinding.
********************************************************************/
struct Node
{
	/*!*******************************************************************
	\enum Status
	\brief
		   Indicates the list which this node is involved.
	********************************************************************/
	enum class Status
	{
		Open,
		Closed,
		Default
	};

	Point m_origin = O; //!< The point where path comes to this node. Simply, it is the middle point of the edge which this node and parent node are sharing.
	Node* m_parent = nullptr; //!< Pointer to parent node. Will be used to restore the path.
	NavMeshManager::NavMesh* m_navMesh = nullptr; //!< Pointer to navigation mesh of this node.

	float m_cost = 0.f; //!< Sum of given cost and heurstic from origin to start point.
	float m_given = 0.f; // The actual cost took to come here from end point.

	Status m_status = Status::Default; //!< Variable stores status of this node.
	int m_iteration = 0; //!< Indicates whether this node has been used in current search. If it has old value, this node needs to be clear.
};

/*!*******************************************************************
\class BinaryHeap
\brief
	   Simple binary searching sorted array.
	   Node has lowest cost will always be first element.
********************************************************************/
class BinaryHeap
{
public:
	/*!*******************************************************************
	\brief
		   Do binary search to find position to insert new node, so
		   keep array be sorted.

	\param node
		   New node to insert. Must not be already in the container.
	********************************************************************/
	void Insert(Node* node);
	/*!*******************************************************************
	\brief
		   Return node has the lowest cost and delete from that
		   from the container.

	\return Node*
			Pointer to node has the lowest cost which is the first
			element of the container.
	********************************************************************/
	Node* GetTop();
	/*!*******************************************************************
	\brief
		   Update given node with given cost and rearrange the container.

	\param node
		   Node to update. Must be in the container already.

	\param new_cost
		   New cost of given node.
	********************************************************************/
	void DecreaseKey(Node* node, float new_cost);
	/*!*******************************************************************
	\brief
		   Telling whether container is empty or not.

	\return bool
			If there is no element in the container, returns true.
			Otherwise, return false.
	********************************************************************/
	bool Empty() const;

private:
	/*!*******************************************************************
	\brief
		   Helper method to find the position of given node inside
		   the container.

	\return int
			Index of the node in the container.
	********************************************************************/
	int Find(Node* node) const;

private:
	std::vector<Node*> m_nodes; //!< Container of nodes which always be sorted by increasing cost order.
};

#endif // !BINARY_HEAP
