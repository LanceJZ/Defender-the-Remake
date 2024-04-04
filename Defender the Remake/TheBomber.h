#pragma once
#include "Enemy.h"
#include "Shot.h"

class TheBomber : public Enemy
{
public:
	TheBomber();
	virtual ~TheBomber();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position);
	void Reset();

private:
	void DropBomb();
	bool CheckCollision();
	void Destroy();
};
