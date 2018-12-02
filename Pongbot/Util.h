#pragma once
#include <cstdint>
#include <vector>

using namespace std;

class Vector;
class IPlayerInfo;

namespace Util {
	int RandomInt(int min, int max);
	void Log(const char *text, ...);
	float DistanceToNoZ(Vector a, Vector b);
	void DrawBeam(Vector startPos, Vector endPos, uint8_t r, uint8_t g, uint8_t b);
	vector<IPlayerInfo*> GetAllPlayers();
}

