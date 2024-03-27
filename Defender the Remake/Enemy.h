#pragma once
#include "Common.h"
#include "Globals.h"
#include "ThePlayer.h"
#include "Shot.h"

class Enemy : public Model3D
{
public:
	Enemy();
	virtual ~Enemy();

	bool CountChange = false;
	int ScoreAmount = 0;
	Shot* Shots[8] = { nullptr };

	void SetPlayer(ThePlayer* player);
	void SetRadarModel(Model model);
	void SetShotModel(Model model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	virtual void Spawn(Vector3 position);

	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void Hit();
	virtual void Reset();

protected:
	size_t ShotTimerID = 0;

	ThePlayer* Player = nullptr;

	virtual void FireShot();
	virtual bool CheckCollision();
	virtual void Destroy();

private:
	float GetShotAngle(Vector3 position);
	float AimedShot(Vector3 position);
};
