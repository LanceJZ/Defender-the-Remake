#include "Shot.h"

Shot::Shot()
{
	LifeTimerID = TheManagers.EM.AddTimer();

	TheManagers.EM.AddModel3D(MirrorL = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(MirrorR = DBG_NEW Model3D());
}

Shot::~Shot()
{
}

void Shot::SetPlayerShotTailModel(Model model)
{
	TheManagers.EM.AddModel3D(PlayerShotTail = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(PlayerShotTailMirrorL = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(PlayerShotTailMirrorR = DBG_NEW Model3D());

	PlayerShotTail->SetModel(model);
	PlayerShotTailMirrorL->SetModel(model);
	PlayerShotTailMirrorR->SetModel(model);

	PlayerShotTail->Position.x = -5.5f;
	PlayerShotTail->SetParent(this);

	float mirrorMultiplier = GetScreenWidth() * 7.0f;

	PlayerShotTailMirrorL->X(PlayerShotTail->X() - mirrorMultiplier);
	PlayerShotTailMirrorR->X(PlayerShotTail->X() + mirrorMultiplier);
	PlayerShotTailMirrorL->SetParent(this);
	PlayerShotTailMirrorR->SetParent(this);

	PlayerShotTailMirrorR->Cull = false; //TODO: See why rotation gets it culled.
}

bool Shot::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	if (PlayerShotTail != nullptr) PlayerShotTail->Initialize(utilities);
	if (PlayerShotTailMirrorL != nullptr) PlayerShotTailMirrorL->Initialize(utilities);
	if (PlayerShotTailMirrorR != nullptr) PlayerShotTailMirrorR->Initialize(utilities);

	MirrorL->Initialize(utilities);
	MirrorR->Initialize(utilities);

	Radius = 3.5f;

	WindowHeight = GetScreenHeight() * 0.5f;

	return true;
}

bool Shot::BeginRun()
{
	Model3D::BeginRun();

	float mirrorMultiplier = GetScreenWidth() * 7.0f;

	MirrorL->SetModel(GetModel());
	MirrorR->SetModel(GetModel());

	MirrorL->X(X() - mirrorMultiplier);
	MirrorR->X(X() + mirrorMultiplier);

	MirrorL->SetParent(this);
	MirrorR->SetParent(this);

	Destroy();

	return true;
}

void Shot::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	CheckPlayfieldSidesWarp(7.0f, 7.0f);

	if (TheManagers.EM.TimerElapsed(LifeTimerID)) Destroy();

	if (Y() > WindowHeight || Y() < -WindowHeight * 0.685f)	Destroy();
}

void Shot::Draw()
{
	Model3D::Draw();
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
	TheManagers.EM.ResetTimer(LifeTimerID, lifeTime);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;
}

void Shot::BombSpawn(Vector3 position, float lifeTime)
{
	Entity::Spawn(position);

	Velocity = { 0, 0, 0 };
	TheManagers.EM.ResetTimer(LifeTimerID, lifeTime);

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
		MirrorR->Cull = false; //TODO: See why rotation gets it culled.
	}
	else
	{
		Velocity.x += 2000.0f;
		Position.x += 40.0f;
		RotationY = 0.0f;
		MirrorR->Cull = true;
	}

	TheManagers.EM.ResetTimer(LifeTimerID, 0.666f);
}
