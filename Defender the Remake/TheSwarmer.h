#pragma once
#include "Globals.h"
#include "Enemy.h"

class TheSwarmer : public Enemy
{
public:
	TheSwarmer();
	virtual ~TheSwarmer();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position);
	void Reset();

private:
	size_t AfterSpawnTimerID = 0;

	void Destroy();

};
