#include "TheBaiter.h"

TheBaiter::TheBaiter()
{
	SpeedChangeTimerID = EM.AddTimer();
	AfterSpawnTimerID = EM.AddTimer();
}

TheBaiter::~TheBaiter()
{
}

bool TheBaiter::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	WorldWidth = FieldSize.x * 0.5f;

	return true;
}

bool TheBaiter::BeginRun()
{
	Enemy::BeginRun();

	Points = 200;

	return false;
}

void TheBaiter::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

}

void TheBaiter::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	if (EM.TimerElapsed(SpeedChangeTimerID))
	{
		ChangeSpeed();
	}

	if (EM.TimerElapsed(AfterSpawnTimerID))
	{
		AfterSpawn();
	}

	CheckPlayfieldHeightWarp(0.679f, 1.0f);

	if (EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
		EM.ResetTimer(ShotTimerID, GetRandomFloat(0.15f, 0.25f));
	}
}

void TheBaiter::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	EM.ResetTimer(SpeedChangeTimerID, 5.5f);
	EM.ResetTimer(AfterSpawnTimerID, 4.0f);
	EM.ResetTimer(ShotTimerID, 1.5f);

	ChangeSpeed();

	Position.x = Player->X();
	Position.y = -GameWindowHalfHeight - Radius;

	Velocity.y = YVelocity;
	Velocity.x = (Player->X() + 0.1f) * -1;
}

void TheBaiter::Reset()
{
}

void TheBaiter::AfterSpawn()
{
	float percentChange = 0.35f;

	if (Player->X() + (GameWindowHalfWidth * percentChange) < X())
	{
		Velocity.x = -XVelocity;
	}
	else if (Player->X() - (GameWindowHalfWidth * percentChange) > X())
	{
		Velocity.x = XVelocity;
	}

	if (Player->Position.y + (GameWindowHalfHeight * percentChange) < Position.y)
	{
		Velocity.y = -YVelocity;
	}
	else if (Player->Position.y - (GameWindowHalfHeight * percentChange) > Position.y)
	{
		Velocity.y = YVelocity;
	}

	if (X() - Player->X() > WorldWidth || Player->X() - X() > WorldWidth)
	{
		Velocity.x = Velocity.x * -1.0f;
	}
}

void TheBaiter::ChangeSpeed()
{
	EM.ResetTimer(SpeedChangeTimerID, GetRandomFloat(2.75f, 5.5f));
	float multiplier = 1.2f;
	XVelocity = (Player->Velocity.x + 0.1f) * multiplier;
	YVelocity = GetRandomFloat(20, 80);
}

void TheBaiter::FireShot()
{
	Enemy::FireShot();

	EM.ResetTimer(ShotTimerID, GetRandomFloat(0.25f, 0.5f));
}

bool TheBaiter::CheckCollision()
{
	Enemy::CheckCollision();

	return false;
}

void TheBaiter::Destroy()
{
	Enemy::Destroy();
}
