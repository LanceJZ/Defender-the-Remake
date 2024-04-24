#include "ThePerson.h"

ThePerson::ThePerson()
{
}

ThePerson::~ThePerson()
{
}

bool ThePerson::Initialize(Utilities* utilities)
{
	MirrorRadar::Initialize(utilities);

	return false;
}

bool ThePerson::BeginRun()
{
	MirrorRadar::BeginRun();

	Radius = VerticesSize * 2.0f;

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

void ThePerson::Draw()
{
	MirrorRadar::Draw();
}

void ThePerson::Spawn(Vector3 position)
{
	MirrorRadar::Spawn(position);

	State = PersonState::OnGround;
	SpawnY = position.y;
}

void ThePerson::Dropped()
{
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
			TheManagers.PM.SpawnExplosion(Position, { 0.25f, 0.25f, 0.25f },
				VerticesSize, 20.0f, 100, 5.0f, { 255, 80, 70, 255 });
			//PlaySound(SplatSound);
			Destroy();
			Player->EnemyUpdate = true;
		}
		else
		{
			//PlaySound(LandedSound);
			//Score->AddToScore(ScoreLandedAmount);
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
		//PlaySound(LeftSound);
		//Score->AddToScore(ScoreDroppedAmount);
		State = OnGround;
		Y(SpawnY);
	}

}

void ThePerson::CheckCollision()
{
	if (CirclesIntersect(*Player) && Player->Enabled)
	{
		//PlaySound(CaughtSound);
		//Score->AddToScore(ScoreCaughtAmount);
		State = CaughtByPlayer;
		Velocity.y = 0;
		Acceleration.y = 0;
	}

}
