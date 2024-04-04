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

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	virtual void Spawn(Vector3 position);

	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void Hit();
	virtual void Reset();

protected:
	size_t ShotTimerID = 0;

	virtual void FireShot();
	virtual bool CheckCollision();
	virtual void Destroy();

private:

	float GetShotAngle(Vector3 position);
	float AimedShot(Vector3 position);
};
