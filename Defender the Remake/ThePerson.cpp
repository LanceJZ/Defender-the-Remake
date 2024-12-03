#include "ThePerson.h"

ThePerson::ThePerson()
{
}

ThePerson::~ThePerson()
{
}

void ThePerson::SetCaughtSound(Sound sound)
{
	CaughtSound = sound;
}

void ThePerson::SetSplatSound(Sound sound)
{
	SplatSound = sound;
}

void ThePerson::SetLandedSound(Sound sound)
{
	LandedSound = sound;
}

void ThePerson::SetDroppedSound(Sound sound)
{
	DroppedSound = sound;
}

bool ThePerson::Initialize(Utilities* utilities)
{
	MirrorRadar::Initialize(utilities);

	return false;
}

bool ThePerson::BeginRun()
{
	MirrorRadar::BeginRun();

	return true;
}

void ThePerson::Update(float deltaTime)
{
	MirrorRadar::Update(deltaTime);

	if (State == PersonState::GoingDown)
	{
		Falling();
	}
	else if (State == PersonState::CaughtByPlayer)
	{
		GoingForARide();
	}
}

void ThePerson::Spawn(Vector3 position)
{
	MirrorRadar::Spawn(position);

	State = PersonState::OnGround;
	SpawnY = position.y;
}

void ThePerson::Dropped()
{
	PlaySound(DroppedSound);
	State = PersonState::GoingDown;
	Velocity.y = 0.0f;
	Acceleration.y = 0.15f;
	DroppedY = Y();
}

void ThePerson::Destroy()
{
	MirrorRadar::Destroy();

}

void ThePerson::Reset()
{
	Y(SpawnY);
}

void ThePerson::Falling()
{
	if (Velocity.y < -60.0f)
	{
		Acceleration.y = 0;
	}

	if (Y() > SpawnY)
	{
		Velocity.y = 0;
		Acceleration.y = 0;
		State = OnGround;

		if (DroppedY < 0)
		{
			Particles.SpawnCubes(Position, { 0.25f, 0.25f, 0.25f },
				VerticesSize, 20.0f, 100, 5.0f, { 255, 80, 70, 255 });
			PlaySound(SplatSound);
			Destroy();
			Player->EnemyUpdate = true;
		}
		else
		{
			PlaySound(LandedSound);
			Player->ScoreUpdate(PointsLanded);
		}
	}

	CheckCollision();
}

void ThePerson::GoingForARide()
{
	X(Player->X());
	Y(Player->Y() + VerticesSize * 4.0f);

	if (Y() > SpawnY - (VerticesSize * 3.0f))
	{
		PlaySound(LandedSound);
		Player->ScoreUpdate(PointsPlayerDroppedOff);
		State = OnGround;
		Y(SpawnY);
	}

}

void ThePerson::CheckCollision()
{
	if (!Player->Enabled) return;

	if (CirclesIntersect(*Player))
	{
		PlaySound(CaughtSound);
		Player->ScoreUpdate(PointsCaught);
		State = CaughtByPlayer;
		Velocity.y = 0;
		Acceleration.y = 0;
	}
}
