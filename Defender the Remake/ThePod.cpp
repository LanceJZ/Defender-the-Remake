#include "ThePod.h"

ThePod::ThePod()
{
}

ThePod::~ThePod()
{
}

bool ThePod::Initialize()
{
	Enemy::Initialize();

	return false;
}

bool ThePod::BeginRun()
{
	Enemy::BeginRun();

	Points = 1000;

	return false;
}

void ThePod::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

}

void ThePod::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	CheckPlayfieldHeightWarp(0.679f, 1.0f);
}

void ThePod::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	if (GetRandomValue(0, 1) == 0)
	{
		Position.x = M.GetRandomFloat(-GetScreenWidth() * 3.5f,
			-GetScreenWidth() * 1.5f);


		Velocity.x = M.GetRandomFloat(35.0f, 55.0f);
	}
	else
	{
		Position.x = M.GetRandomFloat(GetScreenWidth() * 1.5f,
			GetScreenWidth()	* 3.5f);

		Velocity.x = -M.GetRandomFloat(35.0f, 55.0f);
	}

	if (GetRandomValue(0, 1) == 0)
	{
		Velocity.y = M.GetRandomFloat(25.0f, 35.0f);
	}
	else
	{
		Velocity.y = -M.GetRandomFloat(25.0f, 35.0f);
	}

	Position.y = M.GetRandomFloat(-GetScreenHeight() * 0.5f, GetScreenHeight() * 0.5f);
}

void ThePod::Reset()
{
	Enemy::Reset();
	Destroy();
}

void ThePod::Destroy()
{
	Enemy::Destroy();

}

bool ThePod::CheckCollision()
{
	Enemy::CheckCollision();

	return false;
}
