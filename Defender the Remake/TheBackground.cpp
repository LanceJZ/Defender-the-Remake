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

	TheManagers.EM.AddModel3D(UIBackfaceL = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(UIBackfaceR = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(RadarBottom = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(RadarTop = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(RadarLeft = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(RadarRight = DBG_NEW Model3D());
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

void TheBackground::SetUIBackface(Model model)
{
	UIBackfaceL->SetModel(model, 3.05f);
	UIBackfaceR->SetModel(model, 3.05f);
}

void TheBackground::SetRadar(Model horizontal, Model side)
{
	RadarBottom->SetModel(horizontal, 2.18f);
	RadarTop->SetModel(horizontal, 2.18f);
	RadarLeft->SetModel(side, 2.18f);
	RadarRight->SetModel(side, 2.18f);
}

void TheBackground::SetPlayer(ThePlayer* player)
{
	Player = player;
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

	float landY = (GetScreenHeight() / 2.0f) - 160.0f;

	for (int i = 0; i < 7; i++)
	{
		LandParts[i]->Position = { (-GetScreenWidth() * 3.0f) + (GetScreenWidth() * i),
			landY, -40.0f};
	}

	LandParts[7]->Position = { GetScreenWidth() * 4.0f, landY, -40.0f};
	LandParts[8]->Position = { -GetScreenWidth() * 4.0f, landY, -40.0f};

	float UIZ = 50.0f;

	UIBackfaceL->Position = { 0.0f , -GetScreenHeight() / 2.333f, UIZ };
	UIBackfaceR->Position = UIBackfaceL->Position;
	UIBackfaceR->RotationZ = PI;

	RadarTop->Position = { 0.0f, -GetScreenHeight() / 2.02f, UIZ };
	RadarBottom->RotationX = PI;
	RadarBottom->Position = { 0.0f, -GetScreenHeight() / 2.79f, UIZ };
	RadarLeft->Position = RadarBottom->Position;
	RadarRight->Position = RadarBottom->Position;

	return false;
}

void TheBackground::Update()
{
	Common::Update();

	int sWidth = GetScreenWidth();

	RadarBottom->X(TheCamera.position.x);
	RadarTop->X(TheCamera.position.x);
	UIBackfaceL->X(TheCamera.position.x - (sWidth / 2.790f));
	UIBackfaceR->X(TheCamera.position.x + (sWidth / 2.790f));
	RadarLeft->X(TheCamera.position.x - (sWidth / 2.3f));
	RadarRight->X(TheCamera.position.x + (sWidth / 2.3f));
}

void TheBackground::AllThePersonManDead()
{
}

void TheBackground::NewLevel()
{
}

Vector2 TheBackground::UpdateRadar(float x, float y)
{
	float comp = 0.062f;
	float ww = 7.0f;
	float swww = (GetScreenWidth() * ww);
	float swcalc = (swww * comp);
	float swwwcalc = (swww * 2) * comp;

	x = (TheCamera.position.x + (-Player->X() * comp) + (x * comp));

	if (x > TheCamera.position.x + swcalc)
	{
		x = (x - swwwcalc);
	}
	else if (x < TheCamera.position.x - swcalc)
	{
		x = (x + swwwcalc);
	}

	y = ((y * 0.148f) + (GetScreenHeight() * 0.4376f));

	return { x, y };
}

void TheBackground::CreateAllTheStars()
{
}

void TheBackground::UpdateAllTheStars(float deltaTime)
{
}
