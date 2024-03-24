#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
	TheManagers.EM.AddModel3D(Flame = DBG_NEW Model3D());
}

ThePlayer::~ThePlayer()
{
}

void ThePlayer::SetShotModels(Model shotModel, Model tailModel)
{
	ShotModel = shotModel;
	ShotTailModel = tailModel;
}

void ThePlayer::SetFlameModel(Model model)
{
	Flame->SetModel(model);
}

bool ThePlayer::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	return false;
}

bool ThePlayer::BeginRun()
{
	Model3D::BeginRun();

	Flame->SetParent(this);
	Flame->X(-56.0f);
	Reset();

	return false;
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

	ScreenEdgeBoundY(GetScreenHeight() * 0.161f, GetScreenHeight() * 0.015f);
	CheckPlayfieldSidesWarp(4.0f, 3.0f);

	CameraMovement(deltaTime);
}

void ThePlayer::Draw()
{
	Model3D::Draw();

}

void ThePlayer::Hit()
{
	Acceleration = { 0 };
	Velocity = { 0 };
	Lives--;
	Enabled = false;
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
	//Radar.Enabled = true;
	//BackCollusion.Enabled = true;
	//FrontCollusion.Enabled = true;
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

void ThePlayer::CameraMovement(float deltaTime)
{
	float facingOffset = GetScreenWidth() * 0.2f;

	if (ChangedFacing)
	{
		if (FacingRight)
		{
			if (TheCamera.position.x < X() + facingOffset - 0.05f)
			{
				TheCamera.position.x = (X() - facingOffset) + (facingOffset * MoveToOffset);
			}
			else
			{
				ChangedFacing = false;
			}
		}
		else
		{
			if (TheCamera.position.x > X() - facingOffset + 0.05f)
			{
				TheCamera.position.x = (X() + facingOffset) - (facingOffset * MoveToOffset);
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
			TheCamera.position.x = X() + facingOffset;
		}
		else
		{
			TheCamera.position.x = X() - facingOffset;
		}
	}

	TheCamera.target.x = TheCamera.position.x;
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
}