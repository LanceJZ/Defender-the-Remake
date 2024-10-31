#include "TheMutant.h"

TheMutant::TheMutant()
{
	ChangeSpeedTimerID = Managers.EM.AddTimer();
}

TheMutant::~TheMutant()
{
}

bool TheMutant::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	WorldWidth = FieldSize.x * 0.5f;

	return true;
}

bool TheMutant::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheMutant::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	ScreenEdgeBoundY(GetScreenHeight() * 0.15f, VerticesSize * 0.5f);
	ChasePlayer();

	if (Managers.EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
		Managers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.25f, 0.5f));
	}
}

void TheMutant::Spawn(Vector3 position)
{
	Entity::Spawn(position);
}

void TheMutant::ChasePlayer()
{
	float playerX = Player->X();
	float playerY = Player->Y();
	float mutantX = X();
	float mutantY = Y();

	if (Managers.EM.TimerElapsed(ChangeSpeedTimerID))
	{
		Managers.EM.ResetTimer(ChangeSpeedTimerID, GetRandomFloat(0.3f, 2.5f));
		Speed = GetRandomFloat(150.0f, 350.0f);
		DistanceY = GetRandomFloat(100.0f, 150.0f);
	}

	if (mutantX - playerX > WorldWidth || playerX - mutantX > WorldWidth)
	{
		if (playerX < mutantX)
		{
			Velocity.x = Speed;
		}
		else
		{
			Velocity.x = -Speed;
		}
	}
	else
	{
		if (playerX < mutantX)
		{
			Velocity.x = -Speed;
		}
		else
		{
			Velocity.x = Speed;
		}
	}

	if (GotNearPlayer && !BackToTopOrBottom)
	{
		if (playerY + DistanceY < mutantY)
		{
			Velocity.y = -Speed * 0.25f;
		}
		else if (playerY - DistanceY > mutantY)
		{
			Velocity.y = Speed * 0.25f;
		}

		if (playerX + (GetScreenWidth() * 0.5f) < mutantX ||
			playerX + (-GetScreenWidth() * 0.5f) > mutantX)
		{
			BackToTopOrBottom = true;
		}
	}

	if (BackToTopOrBottom)
	{
		if (mutantY < 0)
		{
			Velocity.y = Speed * 0.25f;
		}
		else
		{
			Velocity.y = -Speed * 0.25f;
		}

		if (mutantY > GetScreenHeight() * 0.333f || mutantY < -GetScreenHeight() * 0.45f)
		{
			BackToTopOrBottom = false;
			Velocity.y = 0;
		}
	}

	float distanceX = GetRandomFloat(75.0f, 100.0f);

	if (playerX < mutantX + distanceX && playerX > mutantX - distanceX)
	{
		GotNearPlayer = true;
	}
}

bool TheMutant::CheckCollision()
{
	Enemy::CheckCollision();

	return false;
}
