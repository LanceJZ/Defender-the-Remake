#pragma once
#include "Enemy.h"
#include "Shot.h"

class TheBomber : public Enemy
{
public:
	TheBomber();
	virtual ~TheBomber();

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);

	void Spawn(Vector3 position);
	void Reset();
	void Destroy();

private:
	void DropBomb();
	bool CheckCollision();
};
