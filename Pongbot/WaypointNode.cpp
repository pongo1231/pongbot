#include "WaypointNode.h"

unsigned int _ID;
const Vector Pos;
vector<WaypointNode*> _ConnectedNodes;

WaypointNode::WaypointNode(unsigned int id, Vector pos) : _ID(id), Pos(pos) {}

int WaypointNode::GetID() {
	return _ID;
}

void WaypointNode::ConnectNode(WaypointNode *node) {
	_ConnectedNodes.push_back(node);
}

vector<WaypointNode*> *WaypointNode::GetConnectedNodes() {
	return &_ConnectedNodes;
}