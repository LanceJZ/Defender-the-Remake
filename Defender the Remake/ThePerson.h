#pragma once
#include "MirrorRadar.h"

class ThePerson : public MirrorRadar
{
public:
	enum PersonState
	{
		OnGround,
		GoingDown,
		CaughtByPlayer,
		TargetedByLander
	};

	ThePerson();
	virtual ~ThePerson();

	PersonState State = OnGround;
	bool CountChanged = false;

	void SetCaughtSound(Sound sound);
	void SetSplatSound(Sound sound);
	void SetLandedSound(Sound sound);
	void SetDroppedSound(Sound sound);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void TurnOffRadar();
	void TurnOnRadar();

	void Spawn(Vector3 position);
	void Dropped();
	void Destroy();
	void Reset();

private:
	float DroppedY = 0;
	float SpawnY = 0;

	int PointsPlayerDroppedOff = 500;
	int PointsLanded = 250;
	int PointsCaught = 500;

	Sound CaughtSound = { 0 };
	Sound SplatSound = { 0 };
	Sound LandedSound = { 0 };
	Sound DroppedSound = { 0 };

	void Falling();
	void GoingForARide();
	void CheckCollision();
};
