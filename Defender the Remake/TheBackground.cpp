#include "TheBackground.h"

TheBackground::TheBackground()
{
	StarsTimerID = EM.AddTimer();

	for (size_t i = 0; i < 9; i++)
	{
		EM.AddModel3D(LandParts[i] = DBG_NEW Model3D());
		LandParts[i]->HideCollision = true;
	}

	for (size_t i = 0; i < 14; i++)
	{
		EM.AddModel3D(RadarLandParts[i] = DBG_NEW Model3D());
		RadarLandParts[i]->HideCollision = true;
	}

	EM.AddModel3D(UIBackfaceL = DBG_NEW Model3D());
	EM.AddModel3D(UIBackfaceR = DBG_NEW Model3D());
	EM.AddModel3D(RadarBottom = DBG_NEW Model3D());
	EM.AddModel3D(RadarTop = DBG_NEW Model3D());
	EM.AddModel3D(RadarLeft = DBG_NEW Model3D());
	EM.AddModel3D(RadarRight = DBG_NEW Model3D());
}

TheBackground::~TheBackground()
{
}

void TheBackground::SetLandPartsModel(Model landPart, Model radarLandPart,
	size_t index)
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

bool TheBackground::Initialize()
{
	Common::Initialize();

	return false;
}

bool TheBackground::BeginRun()
{
	Common::BeginRun();

	LandParts[7]->SetModel(LandParts[0]->Get3DModel(), 5.0f);
	LandParts[8]->SetModel(LandParts[6]->Get3DModel(), 5.0f);

	for (int i = 7; i < 14; i++)
	{
		RadarLandParts[i]->SetModel(RadarLandParts[i - 7]->Get3DModel(),
			LandRadarScale);
	}

	float landY = (GameWindowHalfHeight) - 160.0f;

	for (int i = 0; i < 7; i++)
	{
		LandParts[i]->Position = { (-WindowWidth * 3.0f) + (WindowWidth * i),
			landY, 40.0f};
	}

	LandParts[7]->Position = { WindowWidth * 4.0f, landY, 40.0f};
	LandParts[8]->Position = { -WindowWidth * 4.0f, landY, 40.0f};

	float y = ((LandParts[0]->Position.y * 0.158f) +
		(-WindowHeight * 0.4376f)) + 10.0f;

	for (auto &radar : RadarLandParts)
	{
		radar->Y(y);
		radar->Z(50.0f);
	}

	float UIZ = -50.0f;

	UIBackfaceL->Position = { 0.0f , -WindowHeight / 2.333f, UIZ };
	UIBackfaceL->HideCollision = true;
	UIBackfaceR->Position = UIBackfaceL->Position;
	UIBackfaceR->RotationZ = PI;
	UIBackfaceR->HideCollision = true;

	RadarTop->Position = { 0.0f, -WindowHeight / 2.02f, UIZ };
	RadarTop->HideCollision = true;
	RadarBottom->RotationX = PI;
	RadarBottom->Position = { 0.0f, -WindowHeight / 2.79f, UIZ };
	RadarBottom->HideCollision = true;
	RadarLeft->Position = RadarBottom->Position;
	RadarLeft->HideCollision = true;
	RadarRight->Position = RadarBottom->Position;
	RadarRight->HideCollision = true;

	NumberOfStars = 75;

	for (int i = 0; i < NumberOfStars; i++)
	{
		AllTheStars.push_back(DBG_NEW Star());
		EM.AddModel3D(AllTheStars.back());
		AllTheStars[i]->SetModel(StarModel);
		AllTheStars[i]->HideCollision = true;
		AllTheStars[i]->Scale = 2.5f;
		AllTheStars[i]->RelativeVelocity = M.GetRandomFloat(0.85f, 0.99f);
	}

	PlaceAllTheStars();
	BlinkTheStars();
	BlinkTheStars();
	BlinkTheStars();
	BlinkTheStars();
	Update();

	return false;
}

void TheBackground::Update()
{
	Common::Update();

	RadarBottom->X(TheCamera.position.x);
	RadarTop->X(TheCamera.position.x);
	UIBackfaceL->X(TheCamera.position.x - (WindowWidth / 2.790f));
	UIBackfaceR->X(TheCamera.position.x + (WindowWidth / 2.790f));
	RadarLeft->X(TheCamera.position.x - (WindowWidth / 2.3f));
	RadarRight->X(TheCamera.position.x + (WindowWidth / 2.3f));

	if (AllNotDead)
	{
		for (int i = 0; i < 7; i++)
		{
			RadarLandParts[i]->X(UpdateRadar(LandParts[i]->X()));
		}

		for (int i = 7; i < 11; i++)
		{
			RadarLandParts[i]->X(UpdateRadar(LandParts[i - 7]->X() +
				(WindowWidth * 7)));
		}

		for (int i = 11; i < 14; i++)
		{
			RadarLandParts[i]->X(UpdateRadar(LandParts[i - 7]->X() -
				(WindowWidth * 7)));
		}
	}

	if (EM.TimerElapsed(StarsTimerID))
	{
		EM.ResetTimer(StarsTimerID, M.GetRandomFloat(0.15f, 0.5f));
		BlinkTheStars();
	}
}

void TheBackground::PlaceAllTheStars()
{
	for (const auto& star : AllTheStars)
	{
		float x = M.GetRandomFloat((float)(-WindowWidth),
			(float)(WindowWidth));
		float y = 0.0f;

		if (AllNotDead)
		{
			y = M.GetRandomFloat((float)(-WindowHeight * 0.3f),
				(float)(WindowHeight * 0.15f));
		}
		else
		{
			y = M.GetRandomFloat((float)(-WindowHeight * 0.3f),
				(float)(GameWindowHalfHeight));
		}

		star->Position = { x, y, 100.0f };
		star->ModelColor.r = (unsigned char)GetRandomValue(10, 200);
		star->ModelColor.g = (unsigned char)GetRandomValue(10, 200);
		star->ModelColor.b = (unsigned char)GetRandomValue(10, 200);
		star->RotationVelocityX = M.GetRandomFloat(-16.66f, 16.66f);
		star->RotationVelocityY = M.GetRandomFloat(-16.66f, 16.66f);
		star->RotationVelocityZ = M.GetRandomFloat(-16.66f, 16.66f);
	}
}

void TheBackground::NewWave()
{
	for (const auto& land : LandParts)
	{
		land->Enabled = false;
	}
}

void TheBackground::NewWaveDisplayDone()
{
	if (!AllNotDead) return;

	for (const auto& land : LandParts)
	{
		land->Enabled = true;
	}
}

void TheBackground::WorldExplode()
{
	AllNotDead = false;
	WorldGone = true;
	AllThePersonManDead();
	PlaceAllTheStars();

	float yTop = (float)(WindowWidth * 0.15f);
	float yBottom = (float)(WindowHeight * 0.9f);

	for (int i = 0; i < 50; i++)
	{
		Vector3 position = Vector3();
		Color color = Color();

		position.x = M.GetRandomFloat((float)(-WindowWidth),
			(float)(WindowWidth)) + TheCamera.position.x;
		position.y = M.GetRandomFloat(yTop, yBottom);

		color.r = (unsigned char)GetRandomValue(10, 200);
		color.g = (unsigned char)GetRandomValue(10, 200);
		color.b = (unsigned char)GetRandomValue(10, 200);


		Particles.SpawnCubes(position, { 0.0f, 0.0f, 0.0f },
			50.0f, 50.0f, 50, 6.75f, color);
	}
}

void TheBackground::NewGame()
{
	AllThePersonManNotDead();
	AllNotDead = true;
}

void TheBackground::ParallaxTheStars()
{
	bool sideWarped = false;

	if (Player->SideWarped)
	{
		sideWarped = true;
		Player->SideWarped = false;
	}

	for (const auto& star : AllTheStars)
	{
		star->Velocity.x = Player->Velocity.x * star->RelativeVelocity;

		if (Player->ChangedFacing)
		{
			if (Player->FacingRight)
			{
				star->Position.x += Player->CameraFacingOffset *
					Player->MoveToOffset * GetFrameTime();
			}
			else
			{
				star->Position.x -= Player->CameraFacingOffset *
					Player->MoveToOffset * GetFrameTime();
			}
		}

		if (sideWarped)
		{
			if (Player->X() > 0.0f) star->X((star->X()) +
				(WindowWidth * 7.0f));
			else star->X((star->X()) - (WindowWidth * 7.0f));
		}
		else
		{
			if (star->X() > WindowWidth + TheCamera.position.x)
			{
				star->X(-WindowWidth + TheCamera.position.x);
			}
			else if (star->X() < -WindowWidth + TheCamera.position.x)
			{
				star->X(WindowWidth + TheCamera.position.x);
			}
		}
	}
}

float TheBackground::UpdateRadar(float x)
{
	float comp = 0.064f;
	float ww = 7.0f;
	float swww = (WindowWidth * ww);
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

void TheBackground::StillTheStars()
{
	for (const auto& star : AllTheStars)
	{
		star->Velocity.x = 0.0f;
	}
}

void TheBackground::BlinkTheStars()
{
	size_t changeAmount = GetRandomValue(1, GetRandomValue(20, NumberOfStars));

	for (size_t i = 0; i < changeAmount; i++)
	{
		size_t starIndex = (size_t)GetRandomValue(0, NumberOfStars - 1);

		if (GetRandomValue(0, 1) == 0) AllTheStars[starIndex]->ModelColor.a = 255;
		else AllTheStars[starIndex]->ModelColor.a = 0;
	}
}

void TheBackground::AllThePersonManDead()
{
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
	for (auto land : LandParts)
	{
		land->Enabled = true;
	}

	for (auto radar : RadarLandParts)
	{
		radar->Enabled = true;
	}
}