#include "TheBackground.h"

TheBackground::TheBackground()
{

	for (size_t i = 0; i < 9; i++)
	{
		LandPartsID[i] = Managers.EM.AddModel3D(LandParts[i] = DBG_NEW Model3D());
		LandParts[i]->HideCollision = true;
	}

	for (size_t i = 0; i < 14; i++)
	{
		RadarLandPartsID[i] = Managers.EM.AddModel3D(RadarLandParts[i] =
			DBG_NEW Model3D());
		RadarLandParts[i]->HideCollision = true;
	}

	Managers.EM.AddModel3D(UIBackfaceL = DBG_NEW Model3D());
	Managers.EM.AddModel3D(UIBackfaceR = DBG_NEW Model3D());
	Managers.EM.AddModel3D(RadarBottom = DBG_NEW Model3D());
	Managers.EM.AddModel3D(RadarTop = DBG_NEW Model3D());
	Managers.EM.AddModel3D(RadarLeft = DBG_NEW Model3D());
	Managers.EM.AddModel3D(RadarRight = DBG_NEW Model3D());
}

TheBackground::~TheBackground()
{
}

void TheBackground::SetLandParts(Model landPart, Model radarLandPart, int index)
{
	LandParts[index]->SetModel(landPart, 5.0f);
	RadarLandParts[index]->SetModel(radarLandPart, LandRadarScale);
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

	LandParts[7]->SetModel(LandParts[0]->Get3DModel(), 5.0f);
	LandParts[8]->SetModel(LandParts[6]->Get3DModel(), 5.0f);

	for (int i = 7; i < 14; i++)
	{
		RadarLandParts[i]->SetModel(RadarLandParts[i - 7]->Get3DModel(), LandRadarScale);
	}

	float landY = (GetScreenHeight() / 2.0f) - 160.0f;

	for (int i = 0; i < 7; i++)
	{
		LandParts[i]->Position = { (-GetScreenWidth() * 3.0f) + (GetScreenWidth() * i),
			landY, 40.0f};
	}

	LandParts[7]->Position = { GetScreenWidth() * 4.0f, landY, 40.0f};
	LandParts[8]->Position = { -GetScreenWidth() * 4.0f, landY, 40.0f};

	float y = ((LandParts[0]->Position.y * 0.158f) +
		(-GetScreenHeight() * 0.4376f)) + 10.0f;

	for (auto &radar : RadarLandParts)
	{
		radar->Y(y);
		radar->Z(50.0f);
	}

	float UIZ = -50.0f;

	UIBackfaceL->Position = { 0.0f , -GetScreenHeight() / 2.333f, UIZ };
	UIBackfaceL->HideCollision = true;
	UIBackfaceR->Position = UIBackfaceL->Position;
	UIBackfaceR->RotationZ = PI;
	UIBackfaceR->HideCollision = true;

	RadarTop->Position = { 0.0f, -GetScreenHeight() / 2.02f, UIZ };
	RadarTop->HideCollision = true;
	RadarBottom->RotationX = PI;
	RadarBottom->Position = { 0.0f, -GetScreenHeight() / 2.79f, UIZ };
	RadarBottom->HideCollision = true;
	RadarLeft->Position = RadarBottom->Position;
	RadarLeft->HideCollision = true;
	RadarRight->Position = RadarBottom->Position;
	RadarRight->HideCollision = true;

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

	if (AllNotDead)
	{
		for (int i = 0; i < 7; i++)
		{
			RadarLandParts[i]->X(UpdateRadar(LandParts[i]->X()));
		}

		for (int i = 7; i < 11; i++)
		{
			RadarLandParts[i]->X(UpdateRadar(LandParts[i - 7]->X() +
				(GetScreenWidth() * 7)));
		}

		for (int i = 11; i < 14; i++)
		{
			RadarLandParts[i]->X(UpdateRadar(LandParts[i - 7]->X() -
				(GetScreenWidth() * 7)));
		}
	}
}

void TheBackground::AllThePersonManDead()
{
	AllNotDead = false;

	for (auto land : LandParts)
	{
		land->Enabled = false;
	}

	for (auto radar : RadarLandParts)
	{
		radar->Enabled = false;
	}
}

void TheBackground::AllThePersonManNotDead()
{
	AllNotDead = true;

	for (auto land : LandParts)
	{
		land->Enabled = true;
	}

	for (auto radar : RadarLandParts)
	{
		radar->Enabled = true;
	}
}

void TheBackground::NewLevel()
{
}

float TheBackground::UpdateRadar(float x)
{
	float comp = 0.064f;
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

	return x;
}

void TheBackground::CreateAllTheStars()
{
}

void TheBackground::UpdateAllTheStars(float deltaTime)
{
}
