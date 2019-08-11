#pragma once
#include <hlsdk/public/mathlib/mathlib.h>
#include <vector>
#include <cstdlib>
#include <stdint.h> 

class WaypointNode
{
public:
	WaypointNode(unsigned int id, Vector pos, unsigned int flags, float range, QAngle optimalViewAngle = QAngle(0.f, 0.f, 0.f))
		: Id(id), Pos(pos), Flags(flags), _Range(range), OptimalViewAngle(optimalViewAngle) {}

public:
	const unsigned int Id;
	const Vector Pos;
	unsigned int Flags;
	QAngle OptimalViewAngle;

	std::vector<WaypointNode*> GetConnectedNodes() const;
	bool ConnectToNode(WaypointNode* node, bool bidirectional = false);
	bool IsConnectedToNode(WaypointNode* node, bool directly = true);
	void UnconnectNode(WaypointNode* node, bool bidirectional = false);
	void UnconnectAllNodes(bool bidirectional = false);
	bool SetRange(float range);
	float GetRange() const;

private:
	std::vector<WaypointNode*> _ConnectedNodes;
	float _Range;
};