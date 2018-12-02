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
	vector<WaypointNode*> *GetConnectedNodes();
	void ConnectNode(WaypointNode *node);
private:
	int _ID;
	vector<WaypointNode*> _ConnectedNodes;
};