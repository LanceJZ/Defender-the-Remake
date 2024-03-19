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
	LandParts[index]->SetModel(landPart, 5.0f);
	LandParts[index]->Radius = 128.0f * 5.0f;
	//RadarLandParts[index]->SetModel(radarLandPart);
}

bool TheBackground::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

	return false;
}

bool TheBackground::BeginRun()
{
	Common::BeginRun();

	LandParts[7]->SetModel(LandParts[0]->GetModel(), 5.0f);
	LandParts[7]->Radius = 128.0f * 5.0f;
	LandParts[8]->SetModel(LandParts[6]->GetModel(), 5.0f);
	LandParts[8]->Radius = 128.0f * 5.0f;

	for (auto landPart : LandParts)
	{
		//landPart->ModelScale = 5.0f;
	}

	float landY = (GetScreenHeight() / 2.0f) - 160.0f;

	for (int i = 0; i < 9; i++)
	{
		LandParts[i]->Position = { (-GetScreenWidth() * 4.0f) + (GetScreenWidth() * i),
			landY, -40.0f};
	}

	return false;
}

void TheBackground::Update()
{
	Common::Update();

}