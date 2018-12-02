#include "WaypointNode.h"

unsigned int _ID;
const Vector Pos;
vector<WaypointNode*> _ConnectedNodes;

WaypointNode::WaypointNode(unsigned int id, Vector pos) : _ID(id), Pos(pos) {}

int WaypointNode::GetID() {
	return _ID;
}

bool WaypointNode::ConnectToNode(WaypointNode *node, bool bidirectional) {
	if (node == this)
		return false;

	// Check if already connected
	for (WaypointNode *connectedNode : _ConnectedNodes)
		if (connectedNode == node)
			return false;
	_ConnectedNodes.push_back(node);

	if (bidirectional) {
		// Also check if already connected on other node
		for (WaypointNode *connectedNode : *node->GetConnectedNodes()) {
			if (connectedNode == this)
				return true; // This node connected to other node successfully previously so just return true anyways
		}
		node->ConnectToNode(this);
	}

	return true;
}

vector<WaypointNode*> *WaypointNode::GetConnectedNodes() {
	return &_ConnectedNodes;
}

bool WaypointNode::IsConnectedToNode(WaypointNode *node, bool directly) {
	if (_ConnectedNodes.empty())
		return false;
	else
		for (WaypointNode *connectedNode : _ConnectedNodes)
			if (connectedNode == node || (!directly && connectedNode->IsConnectedToNode(node, directly)))
				return true;
	return false;
}