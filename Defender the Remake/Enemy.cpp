#include "Enemy.h"

Enemy::Enemy()
{
	for (size_t i = 0; i < 12; i++)
	{
		TheManagers.EM.AddModel3D(Shots[i] = DBG_NEW Shot());
	}

	ShotTimerID = TheManagers.EM.AddTimer();
}

Enemy::~Enemy()
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
	MirrorRadar::Initialize(utilities);

	for(auto shot : Shots)
	{
		shot->Initialize(utilities);
	}


	return true;
}

bool Enemy::BeginRun()
{
	MirrorRadar::BeginRun();

	for (auto shot : Shots)
	{
		shot->BeginRun();
	}

	return false;
}

void Enemy::Update(float deltaTime)
{
	MirrorRadar::Update(deltaTime);

	CheckPlayfieldSidesWarp(7.0f, 7.0f);

	//if (CheckCollision()) Destroy();
	CheckCollision();

}

void Enemy::Draw()
{
	MirrorRadar::Draw();

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
	for (auto &shot : Player->Shots)
	{
		if (shot->CirclesIntersectBullet(*this))
		{
			shot->Reset();
			Hit();
			Destroy();

			return true;
		}
	}

	return false;
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

float Enemy::AimedShot(Vector3 position)
{
	float percentChance = GetRandomFloat(0.0f, 0.05f);

	Vector3 aimXVelocity = Player->Position;
	aimXVelocity.x += Player->Velocity.x;

	if (X() - Player->X() > FieldSize.x * 0.5f || Player->X() - X() > FieldSize.x * 0.5f)
	{
		aimXVelocity.x = aimXVelocity.x * -1.0f;
	}

	return AngleFromVectorZ(aimXVelocity) +
		GetRandomFloat(-percentChance, percentChance);
}

void Enemy::Hit()
{
	BeenHit = true;
	//Score->AddToScore(ScoreAmount);
}

void Enemy::Reset()
{
	BeenHit = false;
}

void Enemy::Spawn(Vector3 position)
{
	MirrorRadar::Spawn(position);

}

void Enemy::Destroy()
{
	MirrorRadar::Destroy();

}