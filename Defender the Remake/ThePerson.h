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

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);

	void Spawn(Vector3 position);
	void Dropped();
	void Destroy();
	void Reset();

private:
	float DroppedY = 0;
	float SpawnY = 0;

	void Falling();
	void GoingForARide();
	void CheckCollision();
};
