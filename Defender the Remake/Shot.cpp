#include "Shot.h"

Shot::Shot()
{
	LifeTimerID = TheManagers.EM.AddTimer();
}

Shot::~Shot()
{
}

bool Shot::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	return false;
}

bool Shot::BeginRun()
{
	Model3D::BeginRun();

	return false;
}

void Shot::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

}

void Shot::Draw()
{
	Model3D::Draw();

}

void Shot::EnemySpawn(Vector3 position, Vector3 velocity, float lifeTime)
{
	Entity::Spawn(position);

}

void Shot::PlayerSpawn(Vector3 position, Vector3 velocity, bool facingRight)
{
	Entity::Spawn(position);

}
