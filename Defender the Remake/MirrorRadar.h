#pragma once
#include "Common.h"
#include "Globals.h"
#include "ThePlayer.h"

class MirrorRadar : public Model3D
{
public:
	MirrorRadar();
	virtual ~MirrorRadar();

	void SetPlayer(ThePlayer* player);
	void SetRadarModel(Model model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);

	void Spawn(Vector3 position);
	void Destroy();

protected:
	float HeightMultiplier = 0;
	float WidthOffset = 0;
	float WidthCameraOffset = 0;
	float WidthMirrorOffset = 0;
	float WidthMirrorModifierOffset = 0;

	Model3D* MirrorR = {};
	Model3D* MirrorL = {};
	Model3D* Radar = {};

	ThePlayer* Player = nullptr;

private:


};
