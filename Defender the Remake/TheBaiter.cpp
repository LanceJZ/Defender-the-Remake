#include "TheBaiter.h"

TheBaiter::TheBaiter()
{
	SpeedChangeTimerID = TheManagers.EM.AddTimer();
	AfterSpawnTimerID = TheManagers.EM.AddTimer();
}

TheBaiter::~TheBaiter()
{
}

bool TheBaiter::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	return false;
}

bool TheBaiter::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheBaiter::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	if (TheManagers.EM.TimerElapsed(SpeedChangeTimerID))
	{
		ChangeSpeed();
	}

	if (TheManagers.EM.TimerElapsed(AfterSpawnTimerID))
	{
		AfterSpawn();
	}

	CheckPlayfieldHeightWarp(0.679f, 1.0f);
}

void TheBaiter::Draw()
{
	Enemy::Draw();

}

void TheBaiter::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	TheManagers.EM.ResetTimer(SpeedChangeTimerID, 5.5f);
	TheManagers.EM.ResetTimer(AfterSpawnTimerID, 4.0f);
	TheManagers.EM.ResetTimer(ShotTimerID, 1.5f);

	ChangeSpeed();

	Position.x = Player->X();
	Position.y = WindowHeight + 15.0f;

	Velocity.y = -YVelocity;
	Velocity.x = (Player->X() + 0.1f) * -1;
}

void TheBaiter::Reset()
{
}
//TODO: Implement chase across border.
void TheBaiter::AfterSpawn()
{
	float percentChange = 0.35f;

	if (Player->X() + (WindowWidth * percentChange) < X())
	{
		Velocity.x = -XVelocity;
	}
	else if (Player->X() - (WindowWidth * percentChange) > X())
	{
		Velocity.x = XVelocity;
	}

	if (Player->Position.y + (WindowHeight * percentChange) < Position.y)
	{
		Velocity.y = -YVelocity;
	}
	else if (Player->Position.y - (WindowHeight * percentChange) > Position.y)
	{
		Velocity.y = YVelocity;
	}
}

void TheBaiter::ChangeSpeed()
{
	TheManagers.EM.ResetTimer(SpeedChangeTimerID, GetRandomFloat(2.75f, 5.5f));
	float multiplier = 1.2f;
	XVelocity = (Player->Velocity.x + 0.1f) * multiplier;
	YVelocity = GetRandomFloat(20, 50);
}

void TheBaiter::FireShot()
{
	Enemy::FireShot();

	TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.25f, 0.5f));
}

bool TheBaiter::CheckCollision()
{
	return false;
}

void TheBaiter::Destroy()
{
	Enemy::Destroy();
}
