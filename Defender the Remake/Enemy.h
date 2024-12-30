#pragma once
#include "MirrorRadar.h"
#include "Shot.h"
#include "ThePlayer.h"

class Enemy : public MirrorRadar
{
public:
	Enemy();
	virtual ~Enemy();

	bool CountChange = false;
	int ScoreAmount = 0;
	Shot* Shots[12] = { nullptr };

	void SetShotModel(Model model);
	void SetFireSound(Sound sound);
	void SetExplodeSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Spawn(Vector3 position);

	void Update(float deltaTime);
	void AlwaysUpdate(float deltaTime);
	void FixedUpdate(float deltaTime);

	void LeavePlay(float turnSpeed, float speed);
	void Hit();
	void Reset();
	void Destroy();
	bool CheckShotCollisions();

protected:
	size_t ShotTimerID = 0;

	int Points = 0;

	Sound FireSound = {};
	Sound ExplodeSound = {};

	virtual void FireShot();
	virtual bool CheckCollision();

private:

	float GetShotAngle(Vector3 position);
	float AimedShot(Vector3 position);
};
