#pragma once
#include "Globals.h"
#include "Common.h"
#include "ThePlayer.h"

class TheBackground : public Common
{
public:
	TheBackground();
	virtual ~TheBackground();

	void SetLandPartsModel(Model landPart, Model radarLandPart, int index);
	void SetUIBackfaceModel(Model model);
	void SetRadarModel(Model horizontal, Model side);
	void SetStarModel(Model model);

	void SetPlayer(ThePlayer* player);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

	void AllThePersonManDead();
	void AllThePersonManNotDead();
	void NewLevel();

private:
	bool AllNotDead = true;
	size_t StarsTimerID = 0;
	int NumberOfStars = 0;
	float LandRadarScale = 0.3167f;

	struct Star
	{
		size_t ID = {};
		size_t EdgeID = {};
	};

	std::vector<Star> StarsOffREdge;
	std::vector<Star> StarsOffLEdge;

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
	void PlaceAllTheStars();
	void ChangeTheStars();
};

