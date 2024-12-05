#include "ThePlayer.h"

ThePlayer::ThePlayer()
{
	EM.AddEntity(CollusionBack = DBG_NEW Entity());
	EM.AddEntity(CollusionMidFront = DBG_NEW Entity());
	EM.AddEntity(CollusionFront = DBG_NEW Entity());
	EM.AddEntity(CollusionTip = DBG_NEW Entity());

	EM.AddModel3D(Flame = DBG_NEW Model3D());
	EM.AddModel3D(Radar = DBG_NEW Model3D());

	EM.AddOnScreenText(Score = DBG_NEW TheScore());

	for (int i = 0; i < 4; i++)
	{
		EM.AddModel3D(Shots[i] = DBG_NEW Shot());
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

void ThePlayer::SetFireSound(Sound sound)
{
	FireSound = sound;
}

void ThePlayer::SetExplosionSound(Sound sound)
{
	ExplodeSound = sound;
}

void ThePlayer::SetThrustSound(Sound sound)
{
	ThrustSound = sound;
}

void ThePlayer::SetSmartbombSound(Sound sound)
{
	SmartbombSound = sound;
}

void ThePlayer::SetBonusSound(Sound sound)
{
	BonusSound = sound;
}

bool ThePlayer::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	for(auto shot : Shots)
	{
		shot->Initialize(utilities);
	}

	RadarModifier = GetScreenHeight() * 0.4374f;
	CameraFacingOffset = GetScreenWidth() * 0.2f;

	return true;
}

bool ThePlayer::BeginRun()
{
	Model3D::BeginRun();

	Flame->SetParent(*this);
	Flame->X(-35.0f);
	Flame->Y(-2.0f);

	CollusionBack->SetParent(*this);
	CollusionBack->X(-21.0f);
	CollusionBack->Y(-4.0f);
	CollusionMidFront->SetParent(*this);
	CollusionMidFront->X(13.0f);
	CollusionFront->SetParent(*this);
	CollusionFront->X(25.0f);
	CollusionFront->Y(1.0f);
	CollusionTip->SetParent(*this);
	CollusionTip->X(34.0f);
	CollusionTip->Y(2.0f);

	Radius = 7.0f;

	CollusionBack->Radius = Radius + 6.0f;
	CollusionMidFront->Radius = Radius - 2.0f;
	CollusionFront->Radius = Radius - 3.0f;
	CollusionTip->Radius = Radius - 4.0f;

	for (auto shot : Shots)
	{
		shot->BeginRun();
	}

	SetSoundPitch(ExplodeSound, 0.75f);
	SetSoundVolume(ThrustSound, 0.75f);
	SetSoundVolume(FireSound, 0.5f);
	SetSoundVolume(SmartbombSound, 0.5f);

	Disable();

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

	ScreenEdgeBoundY(GetScreenHeight() * 0.15f, CollusionBack->Radius * 2.0f);

	if (CheckPlayfieldSidesWarp(7.0f, 7.0f)) SideWarped = true;

	HorizontalFriction();

	CameraMovement(deltaTime);
	RadarMovement(deltaTime);
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
	Vector3 velocity = Vector3Multiply(Velocity, { 0.1f, 0.1f, 0.1f });

	Particles.SpawnCubes(Position, velocity,
		VerticesSize, 70.0f, 200, 5.0f, { 255, 20, 80, 255 });

	Particles.SpawnCubes(CollusionBack->GetWorldPosition(),	velocity,
		VerticesSize, 60.0f, 200, 5.0f, { 255, 0, 100, 255 });
	Particles.SpawnCubes(CollusionMidFront->GetWorldPosition(),	velocity,
		VerticesSize, 80.0f, 200, 5.0f, { 155, 80, 200, 255 });
	Particles.SpawnCubes(CollusionFront->GetWorldPosition(), velocity,
		VerticesSize, 30.0f, 200, 5.0f, { 55, 80, 70, 255 });
	Particles.SpawnCubes(CollusionTip->GetWorldPosition(), velocity,
		VerticesSize, 40.0f, 200, 5.0f, { 255, 0, 170, 255 });

	PlaySound(ExplodeSound);

	BeenHit = true;
	Lives--;
	SmartBombs = 4;
	Disable();

	if (Lives < 0) GameOver = true;
}

void ThePlayer::ScoreUpdate(int addToScore)
{
	Score->AddToScore(addToScore);
	EnemyUpdate = true;

	if (Score->GetScore() > NextNewLifeScore)
	{
		NextNewLifeScore += 10000;
		Lives++;
		NewLife = true;
	}
}

void ThePlayer::Reset()
{
	Entity::Reset();

	RotationY = (PI * 2);
	RotationVelocityY = 0;
	FacingRight = true;
	Spawned = true;
	Enabled = true;
	Radar->Enabled = true;
	CollusionBack->Enabled = true;
	CollusionMidFront->Enabled = true;
	CollusionFront->Enabled = true;
	CollusionTip->Enabled = true;
	ThrustOff();

	for (const auto& shot : Shots)
	{
		shot->Reset();
	}
}

void ThePlayer::Disable()
{
	Velocity = { 0.0f, 0.0f, 0.0f };
	CollusionBack->Enabled = false;
	CollusionMidFront->Enabled = false;
	CollusionFront->Enabled = false;
	CollusionTip->Enabled = false;
	Flame->Enabled = false;
	Radar->Enabled = false;
	Enabled = false;
}

void ThePlayer::NewGame()
{
	Lives = 4;
	SmartBombs = 4;
	NextNewLifeScore = 10000;
	GameOver = false;
	Score->ClearScore();
	Reset();
}

void ThePlayer::Thrust()
{
	if (!IsSoundPlaying(ThrustSound)) PlaySound(ThrustSound);

	Flame->RotationVelocityX = 35.0f;

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
	StopSound(ThrustSound);
}

void ThePlayer::Reverse()
{
	float rotateSpeed = 3.25f;

	if (FacingRight)
	{
		FacingRight = false;
		RotationVelocityY = rotateSpeed;
	}
	else
	{
		FacingRight = true;
		RotationVelocityY = -rotateSpeed;
	}

	MoveToOffset = 0.01f;
	RotateFacing = true;
	ChangedFacing = true;
}

void ThePlayer::RotateShipFacing()
{

	if (FacingRight)
	{
		if (RotationY < 0.15f)
		{
			RotateFacing = false;
			RotationVelocityY = 0;
			RotationY = 0.0f;
		}
	}
	else
	{
		if (RotationY < PI && RotationY > (PI - 0.15f))
		{
			RotateFacing = false;
			RotationVelocityY = 0;
			RotationY = PI;
		}
	}
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
	if (Velocity.y > 0.0f)
	{
		Acceleration.y = -HorzSpeed / (HorzDrag / (Velocity.y * AirDrag));
	}
	else if (Velocity.y < 0.0f)
	{
		Acceleration.y = HorzSpeed / (HorzDrag / -(Velocity.y * AirDrag));
	}
}

void ThePlayer::Fire()
{
	for (auto shot : Shots)
	{
		if (shot->Enabled) continue;

		PlaySound(FireSound);

		shot->PlayerSpawn(Position, Velocity, FacingRight);
		return;
	}
}

void ThePlayer::SmartBomb()
{
	if (SmartBombs <= 0) return;

	SmartBombFired = true;
	SmartBombs--;
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

		MoveToOffset += (2.25f * deltaTime);
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
	if (IsKeyDown(KEY_UP))
	{
		MoveUp();
	}
	else
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

	if (IsKeyPressed(KEY_LEFT_CONTROL) ||
		IsKeyPressed(KEY_SPACE))
	{
		Fire();
	}

	if (IsKeyPressed(KEY_RIGHT_CONTROL))
	{
		SmartBomb();
	}
}