#pragma once
#include "Globals.h"
#include "Common.h"
#include "ThePlayer.h"

class TheBackground : public Common
{
public:
	TheBackground();
	virtual ~TheBackground();

	bool WorldGone = false;

	Model3D* UIBackfaceR = nullptr;
	Model3D* UIBackfaceL = nullptr;
	Model3D* RadarBottom = nullptr;
	Model3D* RadarTop = nullptr;
	Model3D* RadarLeft = nullptr;
	Model3D* RadarRight = nullptr;

	void SetLandPartsModel(Model landPart, Model radarLandPart, size_t index);
	void SetUIBackfaceModel(Model model);
	void SetRadarModel(Model horizontal, Model side);
	void SetStarModel(Model model);

	void SetPlayer(ThePlayer* player);

	bool Initialize();
	bool BeginRun();

	void Update();

	void PlaceAllTheStars();
	void ParallaxTheStars();
	void NewWave();
	void NewWaveDisplayDone();
	void WorldExplode();
	void NewGame();
	void StillTheStars();

private:
	struct Star : public Model3D
	{
		float RelativeVelocity = 0.0f;
	};

	bool AllNotDead = true;
	size_t StarsTimerID = 0;
	int NumberOfStars = 0;
	float LandRadarScale = 0.3167f;

	Model StarModel = {};

	Model3D* LandParts[9] = { nullptr };
	Model3D* RadarLandParts[14] = { nullptr };

	std::vector<Star*> AllTheStars;

	ThePlayer* Player = nullptr;

	float UpdateRadar(float x);

	void BlinkTheStars();
	void AllThePersonManDead();
	void AllThePersonManNotDead();
};