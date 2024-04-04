#pragma once
#include "Enemy.h"

class TheMutant : public Enemy
{
public:
	TheMutant();
	virtual ~TheMutant();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position);

private:
	bool GotNearPlayer = false;
	bool BackToTopOrBottom = false;
	size_t ChangeSpeedTimerID = 0;
	float Speed = 100;
	float DistanceY = 100;

	void ChasePlayer();
	bool CheckCollision();
};
