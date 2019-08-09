#pragma once
#include "../IConVarBase.h"
#include "../IGameFramable.h"
#include "WaypointNode.h"
#include <vector>
#include <stack>

class WaypointManager : public IConVarBase, public IGameFramable
{
private:
	WaypointManager() {}

public:
	static void Init();
	static void Destroy();

	WaypointNode* GetRandomWaypointNode(unsigned int nodeFlagBlacklist = 0) const;
	WaypointNode* GetClosestWaypointNode(Vector pos, float maxDistance = -1, unsigned int nodeFlagWhitelist = 0) const;
	float GetShortestWaypointNodeRouteToTargetNode(WaypointNode* startNode, WaypointNode* targetNode,
		std::stack<WaypointNode*>* waypointNodesStack, unsigned int flagBlacklist = 0,
		std::vector<WaypointNode*> _alreadyTraversedWaypointNodesStack = std::vector<WaypointNode*>());
	bool GetRandomWaypointNodeRouteToTargetNode(WaypointNode* startNode, WaypointNode* targetNode,
		std::stack<WaypointNode*>* waypointNodesStack, unsigned int flagBlacklist = 0,
		std::vector<WaypointNode*> _alreadyTraversedWaypointNodesStack = std::vector<WaypointNode*>());
	virtual void OnGameFrame();
};

extern WaypointManager* _WaypointManager;