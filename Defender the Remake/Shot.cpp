#include "Shot.h"

Shot::Shot()
{
	LifeTimerID = TheManagers.EM.AddTimer();

	TheManagers.EM.AddModel3D(MirrorL = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(MirrorR = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(PlayerShotTail = DBG_NEW Model3D());
}

Shot::~Shot()
{
}

void Shot::SetPlayerShotTailModel(Model model)
{
	PlayerShotTail->SetModel(model);
}

bool Shot::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	PlayerShotTail->Initialize(utilities);
	MirrorL->Initialize(utilities);
	MirrorR->Initialize(utilities);

	Radius = 3.5f;

	WindowHeight = GetScreenHeight() * 0.5f;

	return true;
}

bool Shot::BeginRun()
{
	Model3D::BeginRun();

	PlayerShotTail->SetParent(this);
	PlayerShotTail->Position.x = -5.5f;

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

	if (TheManagers.EM.TimerElapsed(LifeTimerID))
	{
		Destroy();
	}

	if (Y() > WindowHeight || Y() < -WindowHeight * 0.685f)
	{
		Destroy();
	}
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
		PlayerShotTail->Enabled = false;
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
	Velocity.x = velocity.x;
	Velocity.y = velocity.y * 0.1f;

	if (facingRight)
	{
		Velocity.x -= 2000.0f;
		Position.x -= 40.0f;
		Position.y += 8.0f;
		RotationY = PI;
	}
	else
	{
		Velocity.x += 2000.0f;
		Position.x += 40.0f;
		Position.y += 8.0f;
		RotationY = 0.0f;
	}

	TheManagers.EM.ResetTimer(LifeTimerID, 0.666f);
}
