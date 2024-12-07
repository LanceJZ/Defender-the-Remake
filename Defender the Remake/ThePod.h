#pragma once
#include "Enemy.h"

class ThePod : public Enemy
{
public:
	ThePod();
	virtual ~ThePod();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);

	void Spawn(Vector3 position);
	void Reset();
	void Destroy();

private:
	bool CheckCollision();
};
