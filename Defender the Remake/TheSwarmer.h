#pragma once
#include "Enemy.h"

class TheSwarmer : public Enemy
{
public:
	TheSwarmer();
	virtual ~TheSwarmer();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);

	void Spawn(Vector3 position);
	void Reset();

private:
	bool AfterSpawnTimer = false;

	size_t AfterSpawnTimerID = 0;
	size_t DistanceTimerID = 0;

	float XVelocity = 0;
	float YVelocity = 0;
	float DistanceX = 0;
	float DistanceY = 0;
	float WorldWidth = 0;

	void FireShot();
	void AfterSpawn();
	bool CheckCollision();
	void Destroy();
};
