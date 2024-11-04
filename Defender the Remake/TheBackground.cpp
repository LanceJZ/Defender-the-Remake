#include "TheBackground.h"

TheBackground::TheBackground()
{
	StarsTimerID = Managers.EM.AddTimer();

	for (size_t i = 0; i < 9; i++)
	{
		Managers.EM.AddModel3D(LandParts[i] = DBG_NEW Model3D());
		LandParts[i]->HideCollision = true;
	}

	for (size_t i = 0; i < 14; i++)
	{
		Managers.EM.AddModel3D(RadarLandParts[i] = DBG_NEW Model3D());
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

void TheBackground::SetLandPartsModel(Model landPart, Model radarLandPart, int index)
{
	LandParts[index]->SetModel(landPart, 5.0f);
	RadarLandParts[index]->SetModel(radarLandPart, LandRadarScale);
}

void TheBackground::SetUIBackfaceModel(Model model)
{
	UIBackfaceL->SetModel(model, 3.05f);
	UIBackfaceR->SetModel(model, 3.05f);
}

void TheBackground::SetRadarModel(Model horizontal, Model side)
{
	RadarBottom->SetModel(horizontal, 2.18f);
	RadarTop->SetModel(horizontal, 2.18f);
	RadarLeft->SetModel(side, 2.18f);
	RadarRight->SetModel(side, 2.18f);
}

void TheBackground::SetStarModel(Model model)
{
	StarModel = model;
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

	if (StarsOffREdge.size() == 0) PlaceAllTheStars();

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

	if (Managers.EM.TimerElapsed(StarsTimerID))
	{
		Managers.EM.ResetTimer(StarsTimerID, GetRandomFloat(0.25f, 0.75f));
		ChangeTheStars();
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

void TheBackground::PlaceAllTheStars()
{
	NumberOfStars = 300;

	for (int i = 0; i < NumberOfStars; i++)
	{
		AllTheStars.push_back(DBG_NEW Model3D());
		Managers.EM.AddModel3D(AllTheStars.back());
		AllTheStars[i]->HideCollision = true;
		AllTheStars[i]->Enabled = false;
		AllTheStars[i]->Scale = 2.5f;

		float x = GetRandomFloat((float)(-GetScreenWidth() * 3.5f),
			(float)(GetScreenWidth() * 3.5f));
		float y = 0.0f;

		if (AllNotDead)
		{
			y = GetRandomFloat((float)(-GetScreenHeight() * 0.3f),
				(float)(GetScreenHeight() * 0.15f));
		}
		else
		{
			y = GetRandomFloat((float)(-GetScreenHeight() * 0.3f),
				(float)(GetScreenHeight()));
		}

		Color color = {(unsigned char)GetRandomValue(10, 200),
			(unsigned char)GetRandomValue(10, 200),
			(unsigned char)GetRandomValue(10, 200), 255 };

		AllTheStars[i]->Position = { x, y, -10.0f };
		AllTheStars[i]->ModelColor = color;

		float rotation = GetRandomFloat(-16.66f, 16.66f);

		AllTheStars[i]->RotationVelocityX = rotation;
		AllTheStars[i]->RotationVelocityY = rotation;
		AllTheStars[i]->RotationVelocityZ = rotation;

		AllTheStars[i]->Enabled = true;
	}

	for (int i = 0; i < NumberOfStars; i++)
	{
		if (AllTheStars[i]->X() > GetScreenWidth() * 2.0f)
		{
			StarsOffREdge.push_back(Star());
			StarsOffREdge.back().ID = i;
		}

		if (AllTheStars[i]->X() < -GetScreenWidth() * 2.0f)
		{
			StarsOffLEdge.push_back(Star());
			StarsOffLEdge.back().ID = i;
		}
	}

	size_t iR = AllTheStars.size();

	for (int i = 0; i < StarsOffREdge.size(); i++)
	{
		AllTheStars.push_back(DBG_NEW Model3D());
		Managers.EM.AddModel3D(AllTheStars.back());
		AllTheStars[iR + i]->HideCollision = true;
		AllTheStars[iR + i]->Enabled = false;
		AllTheStars[iR + i]->Scale = 2.5f;
		AllTheStars[iR + i]->X(AllTheStars[StarsOffREdge[i].ID]->X()
			- (float)(GetScreenWidth() * 7.0f));
		AllTheStars[iR + i]->Y(AllTheStars[StarsOffREdge[i].ID]->Y());
		AllTheStars[iR + i]->Z(-10.0f);
		AllTheStars[iR + i]->ModelColor = AllTheStars[StarsOffREdge[i].ID]->ModelColor;
		AllTheStars[iR + i]->RotationX = AllTheStars[StarsOffREdge[i].ID]->RotationX;
		AllTheStars[iR + i]->RotationY = AllTheStars[StarsOffREdge[i].ID]->RotationY;
		AllTheStars[iR + i]->RotationZ = AllTheStars[StarsOffREdge[i].ID]->RotationZ;
		AllTheStars[iR + i]->Enabled = true;

		StarsOffREdge.at(i).EdgeID = iR + i;
	}

	size_t iL = AllTheStars.size();

	for (int i = 0; i < StarsOffLEdge.size(); i++)
	{
		AllTheStars.push_back(DBG_NEW Model3D());
		Managers.EM.AddModel3D(AllTheStars.back());
		AllTheStars[i + iL]->HideCollision = true;
		AllTheStars[i + iL]->Enabled = false;
		AllTheStars[i + iL]->Scale = 2.5f;
		AllTheStars[iL + i]->X(AllTheStars[StarsOffLEdge[i].ID]->X()
			+ (float)(GetScreenWidth() * 7.0f));
		AllTheStars[iL + i]->Z(-10.0f);
		AllTheStars[iL + i]->Y(AllTheStars[StarsOffLEdge[i].ID]->Y());
		AllTheStars[iL + i]->ModelColor = AllTheStars[StarsOffLEdge[i].ID]->ModelColor;
		AllTheStars[iL + i]->RotationX = AllTheStars[StarsOffLEdge[i].ID]->RotationX;
		AllTheStars[iL + i]->RotationY = AllTheStars[StarsOffLEdge[i].ID]->RotationY;
		AllTheStars[iL + i]->RotationZ = AllTheStars[StarsOffLEdge[i].ID]->RotationZ;
		AllTheStars[iL + i]->Enabled = true;

		StarsOffLEdge.at(i).EdgeID = iL + i;
	}

	for (const auto& star : AllTheStars)
	{
		star->SetModel(StarModel);
	}
}

void TheBackground::ChangeTheStars()
{
	int amount = GetRandomValue(1, 200);

	size_t changeAmount = GetRandomValue(1, amount);

	for (size_t i = 0; i < changeAmount; i++)
	{
		size_t starIndex = (size_t)GetRandomValue(0, NumberOfStars - 1);

		AllTheStars[starIndex]->Enabled = true;

		for (auto star : StarsOffREdge)
		{
			if (star.ID == starIndex)
			{
				AllTheStars[star.EdgeID]->Enabled = true;
			}
		}

		for (auto star : StarsOffLEdge)
		{
			if (star.ID == starIndex)
			{
				AllTheStars[star.EdgeID]->Enabled = true;
			}
		}
	}

	changeAmount = GetRandomValue(1, amount);

	for (size_t i = 0; i < changeAmount; i++)
	{
		size_t starIndex = (size_t)GetRandomValue(0, NumberOfStars - 1);

		AllTheStars[starIndex]->Enabled = false;

		for (auto star : StarsOffREdge)
		{
			if (star.ID == starIndex)
			{
				AllTheStars[star.EdgeID]->Enabled = false;
			}
		}

		for (auto star : StarsOffLEdge)
		{
			if (star.ID == starIndex)
			{
				AllTheStars[star.EdgeID]->Enabled = false;
			}
		}
	}
}
