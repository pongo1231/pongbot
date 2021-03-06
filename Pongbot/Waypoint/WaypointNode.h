#pragma once
#include <hlsdk/public/mathlib/mathlib.h>
#include <vector>
#include <cstdlib>
#include <stdint.h> 

class WaypointNode
{
public:
	WaypointNode(unsigned int id, Vector pos, unsigned int flags, float range, QAngle optimalViewAngle = QAngle(0.f, 0.f, 0.f))
		: _Id(id), _Pos(pos), Flags(flags), _Range(range), OptimalViewAngle(optimalViewAngle) {}

public:
	unsigned int Flags;
	QAngle OptimalViewAngle;

	inline unsigned int GetId() const
	{
		return _Id;
	}
	inline Vector GetPos() const
	{
		return _Pos;
	}
	inline std::vector<WaypointNode*> GetConnectedNodes() const
	{
		return _ConnectedNodes;
	}
	bool ConnectToNode(WaypointNode* node, bool bidirectional = false);
	bool IsConnectedToNode(WaypointNode* node, bool directly = true);
	void UnconnectNode(WaypointNode* node, bool bidirectional = false);
	void UnconnectAllNodes(bool bidirectional = false);
	bool SetRange(float range);
	inline float GetRange() const
	{
		return _Range;
	}

private:
	unsigned int _Id;
	Vector _Pos;
	std::vector<WaypointNode*> _ConnectedNodes;
	float _Range;
};