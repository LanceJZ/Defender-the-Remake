#pragma once
#include "Globals.h"
#include "Model3D.h"

class Shot : public Model3D
{
public:
	Shot();
	virtual ~Shot();

	void SetPlayerShotTailModel(Model model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void EnemySpawn(Vector3 position, Vector3 velocity, float lifeTime);
	void BombSpawn(Vector3 position, float lifeTime);
	void PlayerSpawn(Vector3 position, Vector3 velocity, bool facingRight);

	void Update(float deltaTime);
	void Draw();

	void Reset();

private:
	size_t LifeTimerID = 0;
	float WindowHeight = 0;

	Model3D* MirrorR = {};
	Model3D* MirrorL = {};
	Model3D* PlayerShotTail = {};
	Model3D* PlayerShotTailMirrorR = {};
	Model3D* PlayerShotTailMirrorL = {};

	void Destroy();
};
