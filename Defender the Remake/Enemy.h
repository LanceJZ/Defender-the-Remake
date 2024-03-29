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
	Shot* Shots[12] = { nullptr };

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
	float HeightMultiplier = 0;
	float WidthOffset = 0;
	float WidthCameraOffset = 0;
	float WidthMirrorOffset = 0;
	float WidthMirrorModifierOffset = 0;

	Model3D* MirrorR = {};
	Model3D* MirrorL = {};
	Model3D* Radar = {};

	float GetShotAngle(Vector3 position);
	float AimedShot(Vector3 position);
};
