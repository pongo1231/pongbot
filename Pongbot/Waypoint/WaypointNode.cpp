#include "stdafx.h"
#include "WaypointNode.h"

bool WaypointNode::ConnectToNode(WaypointNode* node, bool bidirectional)
{
	if (!node || node == this)
	{
		return false;
	}

	// Check if already connected
	for (WaypointNode* connectedNode : _ConnectedNodes)
	{
		if (connectedNode == node)
		{
			return false;
		}
	}
	_ConnectedNodes.push_back(node);

	if (bidirectional)
	{
		// Also check if already connected on other node
		for (WaypointNode* connectedNode : node->GetConnectedNodes())
		{
			if (connectedNode == this)
			{
				return true; // This node connected to other node successfully previously so just return true anyways
			}
		}
		node->ConnectToNode(this);
	}

	return true;
}

bool WaypointNode::IsConnectedToNode(WaypointNode* node, bool directly)
{
	if (!node || _ConnectedNodes.empty())
	{
		return false;
	}
	else
		for (WaypointNode* connectedNode : _ConnectedNodes)
		{
			if (connectedNode == node || (!directly && connectedNode->IsConnectedToNode(node, directly)))
			{
				return true;
			}
		}
	return false;
}

void WaypointNode::UnconnectNode(WaypointNode* node, bool bidirectional)
{
	if (bidirectional)
	{
		node->UnconnectNode(this);
	}

	for (uint8_t i = 0; i < _ConnectedNodes.size(); i++)
	{
		if (_ConnectedNodes[i] == node)
		{
			_ConnectedNodes.erase(_ConnectedNodes.begin() + i);
		}
	}
}

void WaypointNode::UnconnectAllNodes(bool bidirectional)
{
	for (WaypointNode* node : _ConnectedNodes)
	{
		UnconnectNode(node, bidirectional);
	}
}

bool WaypointNode::SetRange(float range)
{
	if (range >= 0.f)
	{
		_Range = range;
		return true;
	}

	return false;
}