#include "TheSwarmer.h"

TheSwarmer::TheSwarmer()
{
	AfterSpawnTimerID = Managers.EM.AddTimer();
	DistanceTimerID = Managers.EM.AddTimer();
}

TheSwarmer::~TheSwarmer()
{
}

bool TheSwarmer::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	WorldWidth = FieldSize.x * 0.5f;

	return true;
}

bool TheSwarmer::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheSwarmer::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	if (Managers.EM.TimerElapsed(AfterSpawnTimerID))
	{
		AfterSpawnTimer = true;

		if (XVelocity < 0.0f) XVelocity *= -1.0f;
	}

	if (Managers.EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
	}

	CheckPlayfieldHeightWarp(0.679f, 1.0f);

	if (AfterSpawnTimer) AfterSpawn();
}

void TheSwarmer::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	AfterSpawnTimer = false;

	Velocity.x = GetRandomFloat(100.0f, 250.0f);
	Velocity.y = GetRandomFloat(35.0f, 65.0f);

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

	Managers.EM.ResetTimer(ShotTimerID, 1.5f);
	Managers.EM.ResetTimer(AfterSpawnTimerID, GetRandomFloat(2.75f, 4.5f));
}

void TheSwarmer::Reset()
{
	Enemy::Reset();

	Destroy();
}

void TheSwarmer::FireShot()
{

	Managers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.25f, 0.5f));

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
	float percentChange = 0.5f;

	if (Player->X() + (WindowWidth * percentChange) < X())
	{
		Velocity.x = -XVelocity;
	}
	else if (Player->X() - (WindowWidth * percentChange) > X())
	{
		Velocity.x = XVelocity;
	}

	if (Managers.EM.TimerElapsed(DistanceTimerID))
	{
		Managers.EM.ResetTimer(DistanceTimerID, (GetRandomFloat(0.25f, 0.75f)));
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


	if (Position.x - Player->Position.x > WorldWidth ||
		Player->Position.x - Position.x > WorldWidth)
	{
		if (Player->Position.x < Position.x)
		{
			Velocity.x = XVelocity;
		}
		else
		{
			Velocity.x = -XVelocity;
		}
	}
	else
	{
		if (Player->Position.x < Position.x)
		{
			Velocity.x = -XVelocity;
		}
		else
		{
			Velocity.x = XVelocity;
		}
	}
}

bool TheSwarmer::CheckCollision()
{
	Enemy::CheckCollision();

	return false;
}

void TheSwarmer::Destroy()
{
	Enemy::Destroy();
}
