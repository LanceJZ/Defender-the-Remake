#include "Enemy.h"

Enemy::Enemy()
{
	ShotTimerID = Managers.EM.AddTimer();
}

Enemy::~Enemy()
{
}

void Enemy::SetShotModel(Model model)
{
	for (size_t i = 0; i < 12; i++)
	{
		Managers.EM.AddModel3D(Shots[i] = DBG_NEW Shot(), model);
	}
}

void Enemy::SetFireSound(Sound sound)
{
	FireSound = sound;
}

void Enemy::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
}

bool Enemy::Initialize(Utilities* utilities)
{
	MirrorRadar::Initialize(utilities);

	//Radius = 14.0f;

	return true;
}

bool Enemy::BeginRun()
{
	MirrorRadar::BeginRun();

	return false;
}

void Enemy::Update(float deltaTime)
{
	MirrorRadar::Update(deltaTime);

	CheckPlayfieldSidesWarp(7.0f, 7.0f);

	CheckCollision();
}

void Enemy::FireShot()
{
	for (auto shot : Shots)
	{
		if (shot->Enabled) continue;

		if (!Player->GameOver) PlaySound(FireSound);

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

		if (shot->CirclesIntersectBullet(*this) || shot->CirclesIntersect(*this))
		{
			shot->Reset();
			Hit();

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

	return GetAngleFromVectorZ(aimXVelocity) +
		GetRandomFloat(-percentChance, percentChance);
}

void Enemy::Hit()
{
	BeenHit = true;
	Player->EnemyUpdate = true;
	if (!Player->GameOver) PlaySound(ExplodeSound);
	//Score->AddToScore(ScoreAmount);

	float mirrorMultiplier = GetScreenWidth() * 7.0f;
	Color color = { 200, 150, 255, 255 };
	Vector3 mirrorL	= { X() - mirrorMultiplier, Y(), 0.0f};
	Vector3 mirrorR	= { X() + mirrorMultiplier, Y(), 0.0f};

	Particles.SpawnCubes(Position,
		Vector3Multiply(Velocity, { 0.25f, 0.25f, 0.25f }),
		Radius, 100.0f, 10, 3.0f, color);
	Particles.SpawnCubes(mirrorL,
		Vector3Multiply(Velocity, { 0.25f, 0.25f, 0.25f }),
		Radius, 100.0f, 10, 3.0f, color);

	Particles.SpawnCubes(mirrorR,
		Vector3Multiply(Velocity, { 0.25f, 0.25f, 0.25f }),
		Radius, 100.0f, 10, 3.0f, color);
}

void Enemy::Reset()
{
	BeenHit = false;

	Destroy();
}

void Enemy::Spawn(Vector3 position)
{
	MirrorRadar::Spawn(position);

}

void Enemy::Destroy()
{
	MirrorRadar::Destroy();

	Player->EnemyUpdate = true;
}