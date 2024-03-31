#include "TheSwarmer.h"

TheSwarmer::TheSwarmer()
{
	AfterSpawnTimerID = TheManagers.EM.AddTimer();
}

TheSwarmer::~TheSwarmer()
{
}

bool TheSwarmer::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	return false;
}

bool TheSwarmer::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheSwarmer::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

}

void TheSwarmer::Draw()
{
	Enemy::Draw();

}

void TheSwarmer::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	Velocity.x = GetRandomFloat(65.0f, 75.0f);
	Velocity.y = GetRandomFloat(55.0f, 65.0f);

	if (GetRandomValue(0, 1) == 0)
	{
		Velocity.y *= -1;
	}

	if (GetRandomValue(0, 1) == 0)
	{
		Velocity.x *= -1;
	}

	TheManagers.EM.ResetTimer(ShotTimerID, 1.5f);
	TheManagers.EM.ResetTimer(AfterSpawnTimerID, GetRandomFloat(0.25f, 0.5f));
}

void TheSwarmer::Reset()
{
	Enemy::Reset();

	Destroy();
}

void TheSwarmer::Destroy()
{
	Enemy::Destroy();
}
