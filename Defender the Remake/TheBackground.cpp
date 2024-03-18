#include "TheBackground.h"

TheBackground::TheBackground()
{

	for (size_t i = 0; i < 9; i++)
	{
		LandPartsID[i] = TheManagers.EM.AddModel3D(LandParts[i] = DBG_NEW Model3D());
	}

	for (size_t i = 0; i < 12; i++)
	{
		RadarLandPartsID[i] = TheManagers.EM.AddModel3D(RadarLandParts[i] = DBG_NEW Model3D());
	}

}

TheBackground::~TheBackground()
{
}

void TheBackground::SetLandParts(Model landPart, Model radarLandPart, int index)
{
	LandParts[index]->SetModel(landPart);
	RadarLandParts[index]->SetModel(radarLandPart);
}

bool TheBackground::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

	return false;
}

bool TheBackground::BeginRun()
{

	return false;
}

void TheBackground::Update()
{
	Common::Update();

}