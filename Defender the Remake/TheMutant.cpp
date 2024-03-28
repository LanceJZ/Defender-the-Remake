#include "TheMutant.h"

TheMutant::TheMutant()
{
	ChangeSpeedTimerID = TheManagers.EM.AddTimer();
}

TheMutant::~TheMutant()
{
}

bool TheMutant::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	return false;
}

bool TheMutant::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheMutant::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	CheckPlayfieldSidesWarp(4.0f, 3.0f);
	ScreenEdgeBoundY(GetScreenHeight() * 0.161f, GetScreenHeight() * 0.015f);
	ChasePlayer();

	if (TheManagers.EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
		TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.25f, 0.5f));
	}
}

void TheMutant::Draw()
{
	Model3D::Draw();

}

void TheMutant::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}

void TheMutant::ChasePlayer()
{
	float worldW = GetScreenWidth() * 3.5f;
	float playerX = Player->X();
	float playerY = Player->Y();
	float mutantX = X();
	float mutantY = Y();

	if (TheManagers.EM.TimerElapsed(ChangeSpeedTimerID))
	{
		TheManagers.EM.ResetTimer(ChangeSpeedTimerID, GetRandomFloat(0.3f, 2.5f));
		Speed = GetRandomFloat(150.0f, 350.0f);
		DistanceY = GetRandomFloat(100.0f, 150.0f);
	}

	if (mutantX - playerX > worldW || playerX - mutantX > worldW)
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
	return false;
}
