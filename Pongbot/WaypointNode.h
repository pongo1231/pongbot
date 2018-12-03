#pragma once
#include <hlsdk/public/mathlib/mathlib.h>
#include <vector>
#include <cstdlib>

using namespace std;

class WaypointNode {
public:
	WaypointNode(uint8_t id, Vector pos);
	~WaypointNode();
public:
	const unsigned int Id;
	const Vector Pos;
public:
	vector<WaypointNode*> *GetConnectedNodes();
	bool ConnectToNode(WaypointNode *node, bool bidirectional = false);
	bool IsConnectedToNode(WaypointNode *node, bool directly = true);
private:
	vector<WaypointNode*> _ConnectedNodes;
};