#pragma once
#include "Globals.h"
#include "Enemy.h"

class ThePod : public Enemy
{
public:
	ThePod();
	virtual ~ThePod();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position);
	void Reset();

private:
	void Destroy();
	bool CheckCollision();
};
