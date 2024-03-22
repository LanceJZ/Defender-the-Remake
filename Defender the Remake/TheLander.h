#pragma once
#include "Globals.h"
#include "Enemy.h"

class TheLander : public Enemy
{
public:
	TheLander();
	virtual ~TheLander();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Spawn(Vector3 position);

	void Update(float deltaTime);
	void Draw();

private:


};
