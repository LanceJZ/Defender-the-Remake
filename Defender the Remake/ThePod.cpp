#include "ThePod.h"

ThePod::ThePod()
{
}

ThePod::~ThePod()
{
}

bool ThePod::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	return false;
}

bool ThePod::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void ThePod::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	CheckPlayfieldHeightWarp(0.679f, 1.0f);
}

void ThePod::Draw()
{
	Enemy::Draw();

}

void ThePod::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	if (GetRandomValue(0, 1) == 0)
	{
		Position.x = GetRandomFloat(-GetScreenWidth() * 3.5f,
			-GetScreenWidth() * 1.5f);


		Velocity.x = GetRandomFloat(35.0f, 55.0f);
	}
	else
	{
		Position.x = GetRandomFloat(GetScreenWidth() * 1.5f,
			GetScreenWidth()	* 3.5f);

		Velocity.x = -GetRandomFloat(35.0f, 55.0f);
	}

	if (GetRandomValue(0, 1) == 0)
	{
		Velocity.y = GetRandomFloat(25.0f, 35.0f);
	}
	else
	{
		Velocity.y = -GetRandomFloat(25.0f, 35.0f);
	}

	Position.y = GetRandomFloat(-GetScreenHeight() * 0.5f, GetScreenHeight() * 0.5f);
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
