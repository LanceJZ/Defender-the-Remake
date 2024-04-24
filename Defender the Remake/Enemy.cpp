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

	Radius = 14.0f;

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
		if (shot->Enabled) continue;

		shot->EnemySpawn(Position,
			GetVelocityFromAngleZ(GetShotAngle(Position), 125.0f), 8.0f);

		return;
	}
}

bool Enemy::CheckCollision()
{
	for (auto &shot : Player->Shots)
	{
		if (!shot->Enabled)	continue;

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
	Player->EnemyUpdate = true;
	//Score->AddToScore(ScoreAmount);

	Color color = { 200, 150, 255, 255 };

	TheManagers.PM.SpawnExplosion(Position,
		Vector3Multiply(Velocity, { 0.25f, 0.25f, 0.25f }),
		Radius, 100.0f, 10.0f, 3.0f, color);

	float mirrorMultiplier = GetScreenWidth() * 7.0f;

	Vector3 mirrorL	= { X() - mirrorMultiplier, Y(), 0.0f};
	Vector3 mirrorR	= { X() + mirrorMultiplier, Y(), 0.0f};

	TheManagers.PM.SpawnExplosion(mirrorL,
		Vector3Multiply(Velocity, { 0.25f, 0.25f, 0.25f }),
		Radius, 100.0f, 10.0f, 3.0f, color);

	TheManagers.PM.SpawnExplosion(mirrorR,
		Vector3Multiply(Velocity, { 0.25f, 0.25f, 0.25f }),
		Radius, 100.0f, 10.0f, 3.0f, color);
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