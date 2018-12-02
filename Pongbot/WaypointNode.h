#pragma once
#include <hlsdk/public/mathlib/mathlib.h>
#include <vector>

using namespace std;

class WaypointNode {
public:
	WaypointNode(unsigned int id, Vector pos);
public:
	const Vector Pos;
public:
	int GetID();
	vector<WaypointNode*> *GetConnectedNodes();
	bool ConnectToNode(WaypointNode *node, bool bidirectional = false);
	bool IsConnectedToNode(WaypointNode *node, bool directly = true);
private:
	int _ID;
	vector<WaypointNode*> _ConnectedNodes;
};