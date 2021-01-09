/*!*******************************************************************
\file		  BinaryHeap.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include "BinaryHeap.h"

void BinaryHeap::Insert(Node* node)
{
	const float cost = node->m_cost;

	int min = 0;
	int max = static_cast<int>(m_nodes.size());
	int mid = (min + max) / 2;

	// Do binary search to find where to insert.
	while (min != max)
	{
		const float mid_cost = m_nodes[mid]->m_cost;

		// If there is a node having same cost with new node, just insert here.
		// We do not need to care order among the nodes having same cost.
		if (cost == mid_cost)
			break;
		else if (mid_cost < cost)
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

	m_nodes.insert(m_nodes.begin() + mid, node);
}

Node* BinaryHeap::GetTop()
{
	auto node = m_nodes[0];
	m_nodes.erase(m_nodes.begin());

	return node;
}

void BinaryHeap::DecreaseKey(Node* node, float new_cost)
{
	// Get the position of the node to update.
	const int index = Find(node);

	int min = 0;
	int max = index; // New cost is always lower than previous cost, so do not need to look nodes after previous position.
	int mid = (min + max) / 2;

	// Do binary search to find where to move.
	while (min != max)
	{
		const float mid_cost = m_nodes[mid]->m_cost;

		// If there is a node having same cost with new cost, just move here.
		// We do not need to care order among the nodes having same cost.
		if (mid_cost == new_cost)
			break;
		else if (mid_cost < new_cost)
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

	// Shift all nodes from old position to new position.
	for (int i = index; i > mid; --i)
		m_nodes[i] = m_nodes[i - 1];

	// Copy the node to the new position.
	node->m_cost = new_cost;
	m_nodes[mid] = node;
}

int BinaryHeap::Find(Node* node) const
{
	const float cost = node->m_cost;

	int min = 0;
	int max = static_cast<int>(m_nodes.size());
	int mid = (min + max) / 2;

	// Do binary search to find where is the given node.
	while (min != max)
	{
		const float mid_cost = m_nodes[mid]->m_cost;

		// If there is a node having same cost with the node we are looking for,
		// try to find that node inside this range.
		if (mid_cost == cost)
			break;
		else if (mid_cost < cost)
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

	// Find node from mid to max. If cost becomse different, stops.
	for (int i = mid; i < max; ++i)
	{
		if (m_nodes[i] == node)
			return i;
		else if (m_nodes[i]->m_cost != node->m_cost)
			break;
	}

	// Find node from mid to mid. If cost becomse different, stops.
	for (int i = mid - 1; i >= min; --i)
	{
		if (m_nodes[i] == node)
			return i;
		else if (m_nodes[i]->m_cost != node->m_cost)
			break;
	}

	// Error
	return -1;
}

bool BinaryHeap::Empty() const
{
	return m_nodes.empty();
}