#pragma once
#include "Globals.h"
#include "Common.h"
#include "ThePlayer.h"

class TheBackground : public Common
{
public:
	TheBackground();
	virtual ~TheBackground();

	void SetLandParts(Model landPart, Model radarLandPart, int index);
	void SetUIBackface(Model model);
	void SetRadar(Model horizontal, Model side);
	void SetPlayer(ThePlayer* player);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

	void AllThePersonManDead();
	void AllThePersonManNotDead();
	void NewLevel();

private:
	bool AllNotDead = true;
	int NumberOfStars = 0;
	float LandRadarScale = 0.3167f;

	size_t LandPartsID[9] = { 0 };
	size_t RadarLandPartsID[14] = { 0 };
	Model3D* LandParts[9] = { nullptr };
	Model3D* RadarLandParts[14] = { nullptr };
	Model3D* AllTheStars[180] = { nullptr };
	Model3D* UIBackfaceR = nullptr;
	Model3D* UIBackfaceL = nullptr;
	Model3D* RadarBottom = nullptr;
	Model3D* RadarTop = nullptr;
	Model3D* RadarLeft = nullptr;
	Model3D* RadarRight = nullptr;

	ThePlayer* Player = nullptr;

	float UpdateRadar(float x);
	void CreateAllTheStars();
	void UpdateAllTheStars(float deltaTime);
};

