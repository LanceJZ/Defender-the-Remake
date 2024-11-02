#include "Shot.h"

Shot::Shot()
{
	LifeTimerID = Managers.EM.AddTimer();
}

Shot::~Shot()
{
}

void Shot::SetPlayerShotTailModel(Model model)
{
	Managers.EM.AddModel3D(PlayerShotTail = DBG_NEW Model3D(), model);
	Managers.EM.AddModel3D(PlayerShotTailMirrorL = DBG_NEW Model3D(), model);
	Managers.EM.AddModel3D(PlayerShotTailMirrorR = DBG_NEW Model3D(), model);

	PlayerShotTail->SetParent(*this);

	float mirrorMultiplier = GetScreenWidth() * 7.0f;

	PlayerShotTailMirrorL->X(PlayerShotTail->X() - mirrorMultiplier);
	PlayerShotTailMirrorR->X(PlayerShotTail->X() + mirrorMultiplier);
	PlayerShotTailMirrorL->SetParent(*this);
	PlayerShotTailMirrorR->SetParent(*this);
}

bool Shot::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	Managers.EM.AddModel3D(MirrorL = DBG_NEW Model3D());
	Managers.EM.AddModel3D(MirrorR = DBG_NEW Model3D());

	if (PlayerShotTail != nullptr) PlayerShotTail->Initialize(utilities);
	if (PlayerShotTailMirrorL != nullptr) PlayerShotTailMirrorL->Initialize(utilities);
	if (PlayerShotTailMirrorR != nullptr) PlayerShotTailMirrorR->Initialize(utilities);

	MirrorL->Initialize(utilities);
	MirrorR->Initialize(utilities);

	WindowHeight = GetScreenHeight() * 0.5f;

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
	CheckPlayfieldSidesWarp(7.0f, 7.0f); //This needs to be before for ray cast.

	Model3D::Update(deltaTime);

	if (Managers.EM.TimerElapsed(LifeTimerID)) Destroy();

	if (Y() > WindowHeight || Y() < -WindowHeight * 0.685f)	Destroy();
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
	Managers.EM.ResetTimer(LifeTimerID, lifeTime);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;
}

void Shot::BombSpawn(Vector3 position, float lifeTime)
{
	Entity::Spawn(position);

	Velocity = { 0, 0, 0 };
	Managers.EM.ResetTimer(LifeTimerID, lifeTime);

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

	Managers.EM.ResetTimer(LifeTimerID, 0.666f);
}
