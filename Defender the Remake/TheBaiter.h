#pragma once
#include "Enemy.h"

class TheBaiter : public Enemy
{
public:
	TheBaiter();
	virtual ~TheBaiter();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);

	void Spawn(Vector3 position);
	void Reset();
	void Destroy();

private:
	size_t AfterSpawnTimerID = 0;
	size_t SpeedChangeTimerID = 0;

	float XVelocity = 0;
	float YVelocity = 0;
	float WorldWidth = 0;

	void AfterSpawn();
	void ChangeSpeed();
	void FireShot();
	bool CheckCollision();
};