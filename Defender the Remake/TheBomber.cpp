#include "TheBomber.h"

TheBomber::TheBomber()
{
}

TheBomber::~TheBomber()
{
}

bool TheBomber::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	return false;
}

bool TheBomber::BeginRun()
{
	Enemy::BeginRun();

	Points = 250;

	return false;
}

void TheBomber::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

}

void TheBomber::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	if (EM.TimerElapsed(ShotTimerID))
	{
		DropBomb();
		EM.ResetTimer(ShotTimerID, GetRandomFloat(1.66f, 5.66f));
	}

	CheckPlayfieldHeightWarp(0.679f, 1.0f);
}

void TheBomber::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	Position.x = GetRandomFloat(GetScreenWidth() * 2.25f, GetScreenWidth() * 3.5f);
	Position.y = GetRandomFloat(-GetScreenHeight() * 0.5f, GetScreenHeight() * 0.5f);

	float yVelocity = GetRandomFloat(30.0f, 75.0f);
	float xVelocity = GetRandomFloat(-75.0f, -25.0f);

	if (GetRandomValue(0, 1) == 0)
	{
		yVelocity = -yVelocity;
	}

	Velocity = { xVelocity, yVelocity, 0.0f };
}

void TheBomber::Reset()
{
	Enemy::Reset();

}

void TheBomber::DropBomb()
{
	for (auto shot : Shots)
	{
		if (!shot->Enabled)
		{
			shot->BombSpawn(Position, GetRandomFloat(6.66f, 16.66f));
			return;
		}
	}
}

bool TheBomber::CheckCollision()
{
	Enemy::CheckCollision();

	return false;
}

void TheBomber::Destroy()
{
	Enemy::Destroy();

}