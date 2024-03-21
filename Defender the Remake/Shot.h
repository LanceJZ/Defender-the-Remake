#pragma once
#include "Globals.h"

class Shot : public Model3D
{
public:
	Shot();
	virtual ~Shot();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void EnemySpawn(Vector3 position, Vector3 velocity, float lifeTime);
	void PlayerSpawn(Vector3 position, Vector3 velocity, bool facingRight);

	void Update(float deltaTime);
	void Draw();

private:
	size_t LifeTimerID = 0;


};
