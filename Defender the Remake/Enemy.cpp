#include "Enemy.h"

Enemy::Enemy()
{
	for (size_t i = 0; i < 8; i++)
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

	CheckPlayfieldSidesWarp(4.0f, 3.0f);
	CheckCollision();
}

void Enemy::Draw()
{
	Model3D::Draw();

}

void Enemy::FireShot()
{
	for (auto shot : Shots)
	{
		if (!shot->Enabled)
		{
			shot->EnemySpawn(Position,
				GetVelocityFromAngleZ(GetShotAngle(Position), 125.0f), 8.0f);
			return;
		}
	}
}

bool Enemy::CheckCollision()
{
	return false;
}

void Enemy::Destroy()
{
}

float Enemy::GetShotAngle(Vector3 position)
{
	float angle = 0;

	if (GetRandomValue(0, 100) > 50)
	{
		angle = AimedShot(position);
	}
	else
	{
		angle = GetRandomRadian();
	}

	return angle;
}
//TODO: add aiming for other side when player near edge.
float Enemy::AimedShot(Vector3 position)
{
	float percentChance = GetRandomFloat(0.0f, 0.05f);

	Vector3 aimXVelocity = Player->Position;
	aimXVelocity.x += Player->Velocity.x;

	return AngleFromVectorZ(aimXVelocity) +
		GetRandomFloat(-percentChance, percentChance);
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