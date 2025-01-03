#include "TheMutant.h"

TheMutant::TheMutant()
{
	ChangeSpeedTimerID = EM.AddTimer();
	ChasePlayerTimerID = EM.AddTimer();
}

TheMutant::~TheMutant()
{
}

bool TheMutant::Initialize()
{
	Enemy::Initialize();

	WorldWidth = FieldSize.x * 0.5f;
	Points = 150;

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

}

void TheMutant::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	ScreenEdgeBoundY(GetScreenHeight() * 0.15f, VerticesSize * 0.5f);

	if (EM.TimerElapsed(ChasePlayerTimerID))
	{
		EM.ResetTimer(ChasePlayerTimerID, M.GetRandomFloat(0.25f, 1.75f));
		ChasePlayer();

	}

	if (EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
		EM.ResetTimer(ShotTimerID, M.GetRandomFloat(0.25f, 0.5f));
	}
}

void TheMutant::Spawn(Vector3 position)
{
	Enemy::Spawn(position);
}

void TheMutant::ChasePlayer()
{
	float playerX = Player->X();
	float playerY = Player->Y();
	float mutantX = X();
	float mutantY = Y();

	if (EM.TimerElapsed(ChangeSpeedTimerID))
	{
		EM.ResetTimer(ChangeSpeedTimerID, M.GetRandomFloat(0.3f, 2.5f));
		Speed = M.GetRandomFloat(150.0f, 350.0f);
		DistanceY = M.GetRandomFloat(100.0f, 150.0f);
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

	float distanceX = M.GetRandomFloat(75.0f, 100.0f);

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
