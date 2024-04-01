#include "TheSwarmer.h"

TheSwarmer::TheSwarmer()
{
	AfterSpawnTimerID = TheManagers.EM.AddTimer();
	DistanceTimerID = TheManagers.EM.AddTimer();
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

	if (TheManagers.EM.TimerElapsed(AfterSpawnTimerID))
	{
		AfterSpawn();
	}

	if (TheManagers.EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
	}

	CheckPlayfieldHeightWarp(0.679f, 1.0f);
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

	XVelocity = Velocity.x;
	YVelocity = Velocity.y;

	if (GetRandomValue(0, 1) == 0)
	{
		YVelocity *= -1;
	}

	if (GetRandomValue(0, 1) == 0)
	{
		XVelocity *= -1;
	}

	TheManagers.EM.ResetTimer(ShotTimerID, 1.5f);
	TheManagers.EM.ResetTimer(AfterSpawnTimerID, GetRandomFloat(0.25f, 0.5f));
}

void TheSwarmer::Reset()
{
	Enemy::Reset();

	Destroy();
}

void TheSwarmer::FireShot()
{

	TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.25f, 0.5f));

	if (Velocity.x > 0)
	{
		if (Player->X() < X())
		{
			return;
		}
	}
	else
	{
		if (Player->X() > X())
		{
			return;
		}
	}

	Enemy::FireShot();

}

void TheSwarmer::AfterSpawn()
{
	float percentChange = 0.75f;

	if (Player->X() + (WindowWidth * percentChange) < X())
	{
		Velocity.x = -XVelocity;
	}
	else if (Player->X() - (WindowWidth * percentChange) > X())
	{
		Velocity.x = XVelocity;
	}

	if (TheManagers.EM.TimerElapsed(DistanceTimerID))
	{
		TheManagers.EM.ResetTimer(DistanceTimerID, (GetRandomFloat(0.25f, 0.75f)));
		DistanceX = GetRandomFloat(200.0f, 250.0f);
		DistanceY = GetRandomFloat(25.0f, 50.0f);
	}

	if (Player->X() < X() + DistanceX && Player->X() > X() - DistanceX)
	{
		if (Player->Y() + DistanceY < Y())
		{
			Velocity.y = -YVelocity;
		}
		else if (Player->Y() - DistanceY > Y())

		{
			Velocity.y = YVelocity;
		}
	}
}

bool TheSwarmer::CheckCollision()
{
	return false;
}

void TheSwarmer::Destroy()
{
	Enemy::Destroy();
}
