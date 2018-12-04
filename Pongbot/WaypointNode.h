#pragma once
#include <hlsdk/public/mathlib/mathlib.h>
#include <vector>
#include <cstdlib>

class WaypointNode {
public:
	WaypointNode(uint8_t id, Vector pos);
	~WaypointNode();
public:
	const unsigned int Id;
	const Vector Pos;
public:
	std::vector<WaypointNode*> *GetConnectedNodes();
	bool ConnectToNode(WaypointNode *node, bool bidirectional = false);
	bool IsConnectedToNode(WaypointNode *node, bool directly = true);
private:
	std::vector<WaypointNode*> _ConnectedNodes;
};