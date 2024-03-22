#include "Enemy.h"

Enemy::Enemy()
{
	for (size_t i = 0; i < 4; i++)
	{
		TheManagers.EM.AddModel3D(Shots[i] = DBG_NEW Shot());
	}

	ShotTimerID = TheManagers.EM.AddTimer();
}

Enemy::~Enemy()
{
}

void Enemy::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void Enemy::SetRadarModel(Model model)
{
}

void Enemy::SetShotModel(Model model)
{
	for(auto shot : Shots)
	{
		shot->SetModel(model);
	}
}

bool Enemy::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	return false;
}

bool Enemy::BeginRun()
{
	Model3D::BeginRun();

	return false;
}

void Enemy::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

}

void Enemy::Draw()
{
	Model3D::Draw();

}

void Enemy::Hit()
{
}

void Enemy::Reset()
{
}

void Enemy::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}