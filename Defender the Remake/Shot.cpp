#include "Shot.h"

Shot::Shot()
{
	LifeTimerID = EM.AddTimer();
}

Shot::~Shot()
{
}

void Shot::SetPlayerShotTailModel(Model model)
{
	EM.AddModel3D(PlayerShotTail = DBG_NEW Model3D(), model);
	EM.AddModel3D(PlayerShotTailMirrorL = DBG_NEW Model3D(), model);
	EM.AddModel3D(PlayerShotTailMirrorR = DBG_NEW Model3D(), model);

	PlayerShotTail->SetParent(*this);

	float mirrorMultiplier = GetScreenWidth() * 7.0f;

	PlayerShotTailMirrorL->X(PlayerShotTail->X() - mirrorMultiplier);
	PlayerShotTailMirrorR->X(PlayerShotTail->X() + mirrorMultiplier);
	PlayerShotTailMirrorL->SetParent(*this);
	PlayerShotTailMirrorR->SetParent(*this);
}

bool Shot::Initialize()
{
	Model3D::Initialize();

	EM.AddModel3D(MirrorL = DBG_NEW Model3D());
	EM.AddModel3D(MirrorR = DBG_NEW Model3D());

	if (PlayerShotTail != nullptr) PlayerShotTail->Initialize();
	if (PlayerShotTailMirrorL != nullptr) PlayerShotTailMirrorL->Initialize();
	if (PlayerShotTailMirrorR != nullptr) PlayerShotTailMirrorR->Initialize();

	MirrorL->Initialize();
	MirrorR->Initialize();

	GameWindowHalfHeight = GetScreenHeight() * 0.5f;

	return true;
}

bool Shot::BeginRun()
{
	Model3D::BeginRun();

	float mirrorMultiplier = GetScreenWidth() * 7.0f;

	MirrorL->SetModel(Get3DModel());
	MirrorR->SetModel(Get3DModel());

	MirrorL->X(X() - mirrorMultiplier);
	MirrorR->X(X() + mirrorMultiplier);

	MirrorL->SetParent(*this);
	MirrorR->SetParent(*this);

	Destroy();

	return true;
}

void Shot::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

}

void Shot::FixedUpdate(float deltaTime)
{
	Model3D::FixedUpdate(deltaTime);

	CheckPlayfieldSidesWarp(7.0f, 7.0f); //This needs to be before for ray cast.

	if (EM.TimerElapsed(LifeTimerID)) Destroy();

	if (Y() > GameWindowHalfHeight || Y() < -GameWindowHalfHeight * 0.685f)	Destroy();
}

void Shot::Reset()
{
	Destroy();
}

void Shot::Destroy()
{
	Enabled = false;
	MirrorL->Enabled = false;
	MirrorR->Enabled = false;

	if (PlayerShotTail != nullptr) PlayerShotTail->Enabled = false;
	if (PlayerShotTailMirrorL != nullptr) PlayerShotTailMirrorL->Enabled = false;
	if (PlayerShotTailMirrorR != nullptr) PlayerShotTailMirrorR->Enabled = false;
}

void Shot::EnemySpawn(Vector3 position, Vector3 velocity, float lifeTime)
{
	Entity::Spawn(position);

	Velocity = velocity;
	EM.ResetTimer(LifeTimerID, lifeTime);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;
}

void Shot::BombSpawn(Vector3 position, float lifeTime)
{
	Entity::Spawn(position);

	Velocity = { 0, 0, 0 };
	EM.ResetTimer(LifeTimerID, lifeTime);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;
}

void Shot::PlayerSpawn(Vector3 position, Vector3 velocity, bool facingRight)
{
	Entity::Spawn(position);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;

	PlayerShotTail->Enabled = true;
	PlayerShotTailMirrorL->Enabled = true;
	PlayerShotTailMirrorR->Enabled = true;

	Velocity.x = velocity.x;
	Velocity.y = velocity.y * 0.1f;
	Position.y += 2.0f;

	if (!facingRight)
	{
		Velocity.x -= 2000.0f;
		Position.x -= 40.0f;
		RotationY = PI;
	}
	else
	{
		Velocity.x += 2000.0f;
		Position.x += 40.0f;
		RotationY = 0.0f;
	}

	EM.ResetTimer(LifeTimerID, 0.666f);
}
