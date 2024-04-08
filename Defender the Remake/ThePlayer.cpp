#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
	TheManagers.EM.AddEntity(CollusionBack = DBG_NEW Entity());
	TheManagers.EM.AddEntity(CollusionMidFront = DBG_NEW Entity());
	TheManagers.EM.AddEntity(CollusionFront = DBG_NEW Entity());
	TheManagers.EM.AddEntity(CollusionTip = DBG_NEW Entity());

	TheManagers.EM.AddModel3D(Flame = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(Radar = DBG_NEW Model3D());

	for (int i = 0; i < 4; i++)
	{
		TheManagers.EM.AddModel3D(Shots[i] = DBG_NEW Shot());
	}
}

ThePlayer::~ThePlayer()
{
}

void ThePlayer::SetShotModels(Model shotModel, Model tailModel)
{
	for(auto shot : Shots)
	{
		shot->SetModel(shotModel);
		shot->SetPlayerShotTailModel(tailModel);
	}
}

void ThePlayer::SetFlameModel(Model model)
{
	Flame->SetModel(model);
}

void ThePlayer::SetRadarModel(Model model)
{
	Radar->SetModel(model);
}

bool ThePlayer::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	for(auto shot : Shots)
	{
		shot->Initialize(utilities);
	}

	Radar->Initialize(utilities);
	RadarModifier = GetScreenHeight() * 0.4374f;
	CameraFacingOffset = GetScreenWidth() * 0.2f;

	Flame->Initialize(utilities);

	Radius = 7.0f;

	CollusionBack->Radius = Radius + 6.0f;
	CollusionMidFront->Radius = Radius - 2.0f;
	CollusionFront->Radius = Radius - 3.0f;
	CollusionTip->Radius = Radius - 4.0f;

	return true;
}

bool ThePlayer::BeginRun()
{
	Model3D::BeginRun();

	Flame->BeginRun();
	Radar->BeginRun();

	Flame->SetParent(this);
	Flame->X(-56.0f);

	CollusionBack->SetParent(this);
	CollusionBack->X(-21.0f);
	CollusionBack->Y(-4.0f);
	CollusionMidFront->SetParent(this);
	CollusionMidFront->X(13.0f);
	CollusionFront->SetParent(this);
	CollusionFront->X(25.0f);
	CollusionFront->Y(1.0f);
	CollusionTip->SetParent(this);
	CollusionTip->X(34.0f);
	CollusionTip->Y(2.0f);

	Reset();

	return true;
}

void ThePlayer::Input()
{
	Model3D::Input();

	Keyboard();

	if (IsGamepadAvailable(0))
	{
		Gamepad();
	}
}

void ThePlayer::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	if (RotateFacing)
		RotateShipFacing();

	ScreenEdgeBoundY(GetScreenHeight() * 0.015f, GetScreenHeight() * 0.161f);
	CheckPlayfieldSidesWarp(7.0f, 7.0f);

	HorizontalFriction();

	CameraMovement(deltaTime);
	RadarMovement(deltaTime);
}

void ThePlayer::Draw()
{
	Model3D::Draw();

}

bool ThePlayer::GetCollusion(Entity& target)
{
	if (CollusionBack->CirclesIntersect(target) ||
		CollusionMidFront->CirclesIntersect(target) ||
		CollusionFront->CirclesIntersect(target) ||
		CollusionTip->CirclesIntersect(target) ||
		CirclesIntersect(target)) return true;

	return false;
}

void ThePlayer::Hit()
{
	Acceleration = { 0 };
	Velocity = { 0 };
	Lives--;
	Enabled = false;
	BeenHit = true;
	CollusionBack->Enabled = false;
	CollusionMidFront->Enabled = false;
	CollusionFront->Enabled = false;
	CollusionTip->Enabled = false;
}

void ThePlayer::ScoreUpdate(int addToScore)
{
	Score += addToScore;

	if (Score > HighScore)
	{
		HighScore = Score;
	}

	if (Score > NextNewLifeScore)
	{
		NextNewLifeScore += 10000;
		Lives++;
		NewLife = true;
	}
}

void ThePlayer::Reset()
{
	Position = { 0, 0, 0 };
	Velocity = { 0, 0, 0 };
	Enabled = true;
	BeenHit = false;
	ThrustOff();
	Position = { 0, 0, 0 };
	Velocity = { 0, 0, 0 };
	Acceleration = { 0, 0, 0 };
	FacingRight = true;
	RotationY = (PI * 2);
	Enabled = true;
	Radar->Enabled = true;
	CollusionBack->Enabled = true;
	CollusionMidFront->Enabled = true;
	CollusionFront->Enabled = true;
	CollusionTip->Enabled = true;
}

void ThePlayer::NewGame()
{
	Lives = 4;
	NextNewLifeScore = 10000;
	Score = 0;
	GameOver = false;
	Reset();
}

void ThePlayer::Thrust()
{
	if (FacingRight)
	{
		MoveRight();
	}
	else
	{
		MoveLeft();
	}
}

void ThePlayer::ThrustOff()
{
	if (Velocity.x > 0)
	{
		Acceleration.x = -ForwardAcceleration / (ForwardDrag / (Velocity.x * AirDrag));
	}
	else if (Velocity.x < 0)
	{
		Acceleration.x = ForwardAcceleration / (ForwardDrag / -(Velocity.x * AirDrag));
	}

	Flame->Enabled = false;
}

void ThePlayer::Reverse()
{
	if (FacingRight)
	{
		FacingRight = false;
	}
	else
	{
		FacingRight = true;
	}

	MoveToOffset = 0.01f;
	RotateFacing = true;
	ChangedFacing = true;
}

void ThePlayer::MoveLeft()
{
	if (Velocity.x > -MaxForwardV)
	{
		Acceleration.x = -HorzSpeed;
		Flame->Enabled = true;
	}
	else
	{
		Acceleration.x = 0;
	}
}

void ThePlayer::MoveRight()
{
	if (Velocity.x < MaxForwardV)
	{
		Acceleration.x = HorzSpeed;
		Flame->Enabled = true;
	}
	else
	{
		Acceleration.x = 0;
	}
}

void ThePlayer::MoveUp()
{
	if (Velocity.y > -HorzMaxSpeed)
	{
		Acceleration.y = -HorzSpeed;
	}
	else
	{
		Acceleration.y = 0;
	}
}

void ThePlayer::MoveDown()
{
	if (Velocity.y < HorzMaxSpeed)
	{
		Acceleration.y = HorzSpeed;
	}
	else
	{
		Acceleration.y = 0;
	}
}

void ThePlayer::HorizontalFriction()
{
	if (Velocity.y > 0)
	{
		Acceleration.y = -HorzSpeed / (HorzDrag / (Velocity.y * AirDrag));
	}
	else if (Velocity.y < 0)
	{
		Acceleration.y = HorzSpeed / (HorzDrag / -(Velocity.y * AirDrag));
	}
}

void ThePlayer::RotateShipFacing()
{
	float rotateSpeed = 0.045f;

	if (FacingRight)
	{
		if (RotationY < (PI * 2))
		{
			RotationY += rotateSpeed;
		}
		else
		{
			RotateFacing = false;
		}

		//BackCollusion.Position.x = -24.0f;
	}
	else
	{
		if (RotationY > PI)
		{
			RotationY -= rotateSpeed;
		}
		else
		{
			RotateFacing = false;
		}

		//BackCollusion.Position.x = 24.0f;
	}
}

void ThePlayer::Fire()
{
	for (auto shot : Shots)
	{
		if (!shot->Enabled)
		{
			shot->PlayerSpawn(Position, Velocity, !FacingRight);
			return;
		}
	}
}

void ThePlayer::CameraMovement(float deltaTime)
{
	if (ChangedFacing)
	{
		if (FacingRight)
		{
			if (TheCamera.position.x < X() + CameraFacingOffset - 0.05f)
			{
				TheCamera.position.x = (X() - CameraFacingOffset) +
					(CameraFacingOffset * MoveToOffset);
			}
			else
			{
				ChangedFacing = false;
			}
		}
		else
		{
			if (TheCamera.position.x > X() - CameraFacingOffset + 0.05f)
			{
				TheCamera.position.x = (X() + CameraFacingOffset) -
					(CameraFacingOffset * MoveToOffset);
			}
			else
			{
				ChangedFacing = false;
			}
		}

		MoveToOffset += 0.02f;
	}
	else
	{
		if (FacingRight)
		{
			TheCamera.position.x = X() + CameraFacingOffset;
		}
		else
		{
			TheCamera.position.x = X() - CameraFacingOffset;
		}
	}

	TheCamera.target.x = TheCamera.position.x;
}

void ThePlayer::RadarMovement(float deltaTime)
{
	Radar->X(TheCamera.position.x);
	Radar->Y((Y() * 0.158f) - RadarModifier);
}

void ThePlayer::Gamepad()
{
	//Button B is 6 for Shield //Button A is 7 for Fire //Button Y is 8 for Hyperspace
	//Button X is 5	//Left bumper is 9 //Right bumper is 11 for Shield //Left Trigger is 10
	//Right Trigger is 12 for Thrust //Dpad Up is 1 for	//Dpad Down is 3 for
	//Dpad Left is 4 for rotate left //Dpad Right is 2 for rotate right
	//Axis 1 is -1 for , 1 for  on left stick.
	//Axis 0 is -1 for Left, 1 for right on left stick.

	if (IsGamepadButtonDown(0, 12))
	{
	}
	else
	{
	}

	if (IsGamepadButtonDown(0, 4) || GetGamepadAxisMovement(0, 0) < -0.25f)
	{
	}
	else if (IsGamepadButtonDown(0, 2) || GetGamepadAxisMovement(0, 0) > 0.25f)
	{
	}

	if (IsGamepadButtonPressed(0, 7))
	{
	}

	if (IsGamepadButtonDown(0, 11) || IsGamepadButtonDown(0, 6))
	{
	}
	else
	{
	}
}

void ThePlayer::Keyboard()
{
	if (IsKeyDown(KEY_RIGHT))
	{
	}
	else if (IsKeyDown(KEY_LEFT))
	{
	}
	else
	{
	}

	if (IsKeyDown(KEY_UP))
	{
		MoveUp();
	}
	else
	{
	}

	if (IsKeyPressed(KEY_RIGHT_CONTROL) || IsKeyPressed(KEY_LEFT_CONTROL) ||
		IsKeyPressed(KEY_SPACE))
	{
	}

	if (IsKeyDown(KEY_DOWN))
	{
		MoveDown();
	}
	else
	{
	}

	if (IsKeyDown(KEY_LEFT_SHIFT))
	{
		Thrust();
	}
	else
	{
		ThrustOff();
	}

	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT))
	{
		Reverse();
	}

	if (IsKeyPressed(KEY_LEFT_CONTROL))
	{
		Fire();
	}
}