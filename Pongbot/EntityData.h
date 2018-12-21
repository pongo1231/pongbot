#pragma once

class EntityData
{
public:
	EntityData(const char *className, const char *key);

public:
	int GetOffset() const;

private:
	int _Offset;

	void _FindOffset(const char *className, const char *key);
};