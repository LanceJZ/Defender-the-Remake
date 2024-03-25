#pragma once
#include "Globals.h"
#include "ThePlayer.h"

class ThePerson : public Model3D
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

	void SetPlayer(ThePlayer* player);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position);
	void Dropped();
	void Destroy();
	void Reset();

private:
	float DroppedY = 0;

	ThePlayer* Player = {};

};
