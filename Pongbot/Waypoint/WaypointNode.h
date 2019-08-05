#pragma once
#include <hlsdk/public/mathlib/mathlib.h>
#include <vector>
#include <cstdlib>
#include <stdint.h> 

class WaypointNode
{
public:
	WaypointNode(uint8_t id, Vector pos) : Id(id), Pos(pos) {}

public:
	const unsigned int Id;
	const Vector Pos;
	unsigned int Flags;

	std::vector<WaypointNode*> GetConnectedNodes() const;
	bool ConnectToNode(WaypointNode* node, bool bidirectional = false);
	bool IsConnectedToNode(WaypointNode* node, bool directly = true);
	void UnconnectNode(WaypointNode* node, bool bidirectional = false);
	void UnconnectAllNodes(bool bidirectional = false);

private:
	std::vector<WaypointNode*> _ConnectedNodes;
};