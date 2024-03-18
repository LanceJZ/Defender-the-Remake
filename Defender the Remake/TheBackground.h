#pragma once
#include "Globals.h"
#include "Common.h"

class TheBackground : public Common
{
public:
	TheBackground();
	virtual ~TheBackground();

	void SetLandParts(Model landPart, Model radarLandPart, int index);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

private:

	size_t LandPartsID[9] = { 0 };
	size_t RadarLandPartsID[14] = { 0 };
	Model3D* LandParts[9] = { nullptr };
	Model3D* RadarLandParts[14] = { nullptr };
};

