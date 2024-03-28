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

bool Shot::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	Xmultiplier = GetScreenWidth() * 2.75f;
	MirrorMultiplier = GetScreenWidth() * 7.0f;

	MirrorL->SetModel(GetModel());
	MirrorR->SetModel(GetModel());

	MirrorL->X(X() - MirrorMultiplier);
	MirrorR->X(X() + MirrorMultiplier);

	MirrorL->SetParent(this);
	MirrorR->SetParent(this);

	return true;
}

bool Shot::BeginRun()
{
	Model3D::BeginRun();

	return false;
}

void Shot::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	if (TheManagers.EM.TimerElapsed(LifeTimerID))
	{
		Enabled = false;
		MirrorL->Enabled = false;
		MirrorR->Enabled = false;
	}
}

void Shot::Draw()
{
	Model3D::Draw();

}

void Shot::EnemySpawn(Vector3 position, Vector3 velocity, float lifeTime)
{
	Entity::Spawn(position);

	Velocity = velocity;
	TheManagers.EM.ResetTimer(LifeTimerID, lifeTime);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;
}

void Shot::PlayerSpawn(Vector3 position, Vector3 velocity, bool facingRight)
{
	Entity::Spawn(position);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;
}
