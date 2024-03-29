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
	void SetRadarModel(Model model);

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
	float HeightMultiplier = 0;
	float WidthOffset = 0;
	float WidthCameraOffset = 0;
	float WidthMirrorOffset = 0;
	float WidthMirrorModifierOffset = 0;

	Model3D* MirrorR = {};
	Model3D* MirrorL = {};
	Model3D* Radar = {};
	ThePlayer* Player = {};
};
