#include "WaypointNode.h"

const unsigned int Id = 0;
const Vector Pos;
std::vector<WaypointNode*> _ConnectedNodes;

WaypointNode::WaypointNode(uint8_t id, Vector pos) : Id(id), Pos(pos)
{}

WaypointNode::~WaypointNode()
{
	// Remove all connections to this node on delete
	for (WaypointNode *connectedNode : _ConnectedNodes)
	{
		std::vector<WaypointNode*> *connectedNodeConnectedNodes = connectedNode->GetConnectedNodes();
		for (uint8_t i = 0; i < connectedNodeConnectedNodes->size(); i++)
			if ((*connectedNodeConnectedNodes)[i] == this)
				connectedNodeConnectedNodes->erase(connectedNodeConnectedNodes->begin() + i);
	}
}

std::vector<WaypointNode*> *WaypointNode::GetConnectedNodes()
{
	return &_ConnectedNodes;
}

bool WaypointNode::ConnectToNode(WaypointNode *node, bool bidirectional)
{
	if (!node || node == this)
		return false;

	// Check if already connected
	for (WaypointNode *connectedNode : _ConnectedNodes)
		if (connectedNode == node)
			return false;
	_ConnectedNodes.push_back(node);

	if (bidirectional)
	{
		// Also check if already connected on other node
		for (WaypointNode *connectedNode : *node->GetConnectedNodes())
			if (connectedNode == this)
				return true; // This node connected to other node successfully previously so just return true anyways
		node->ConnectToNode(this);
	}

	return true;
}

bool WaypointNode::IsConnectedToNode(WaypointNode *node, bool directly)
{
	if (!node || _ConnectedNodes.empty())
		return false;
	else
		for (WaypointNode *connectedNode : _ConnectedNodes)
			if (connectedNode == node || (!directly && connectedNode->IsConnectedToNode(node, directly)))
				return true;
	return false;
}