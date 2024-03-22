#pragma once
#include "Globals.h"
#include "Enemy.h"

class TheMutant : public Enemy
{
public:
	TheMutant();
	virtual ~TheMutant();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Spawn(Vector3 position);

	void Update(float deltaTime);
	void Draw();

private:


};
