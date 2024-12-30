#include "TheLander.h"

TheLander::TheLander()
{
	SeekTimerID = EM.AddTimer();
}

TheLander::~TheLander()
{
}

void TheLander::SetPersonGrabbedSound(Sound sound)
{
	PersonGrabbedSound = sound;
}

void TheLander::SetMutateSound(Sound sound)
{
	MutateSound = sound;
}

void TheLander::SetPeople(ThePerson* people[10])
{
	for (int i = 0; i < 10; i++)
	{
		People[i] = people[i];
	}
}

bool TheLander::Initialize()
{
	Enemy::Initialize();

	Points = 150;

	return false;
}

bool TheLander::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheLander::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

}

void TheLander::FixedUpdate(float deltaTime)
{
	Enemy::FixedUpdate(deltaTime);

	switch (State)
	{
	case LanderStateList::LoweringToSeek:
		GoToSeek();
		break;
	case LanderStateList::Seek:
		SeekPersonMan();
		break;
	case LanderStateList::FoundPersonMan:
		GoingDown();
		break;
	case LanderStateList::TakePersonMan:
		GrabPersonMan();
		break;
	case LanderStateList::Mutate:
		SpawnMutant();
		break;
	}
}

void TheLander::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	Position.x = M.GetRandomFloat(-FieldSize.x * 0.5f, FieldSize.x * 0.5f);
	Position.y = -M.WindowHeight * 0.333f;

	State = LanderStateList::LoweringToSeek;

	float xSpeed = 50.0f;

	if (GetRandomValue(0, 1) == 0)
	{
		Velocity.x = M.GetRandomFloat(xSpeed, xSpeed * 2.0f);
	}
	else
	{
		Velocity.x = M.GetRandomFloat(-xSpeed * 2.0f, -xSpeed);
	}

	Velocity.y = M.GetRandomFloat(20.0f, 30.0f);
	GroundFloorY = M.GetRandomFloat(20.0f, 100.0f);

	SeekTimerAmountMin = 5.25f;
	SeekTimerAmountMax = 15.5f;
	SeekTimerChance = 10;

	EM.ResetTimer(ShotTimerID, M.GetRandomFloat(15.15f, 25.75f));
}

void TheLander::Reset()
{
	Enemy::Reset();

	Destroy();
}

void TheLander::Hit()
{
	Enemy::Hit();

	if (State == TakePersonMan || State == Mutate)
	{
		People[PersonTargetID]->Dropped();
	}
}

void TheLander::GoToSeek()
{
	if (Y() > (GetScreenHeight() * 0.2f) + GroundFloorY)
	{
		State = LanderStateList::Seek;
	}

	if (EM.TimerElapsed(ShotTimerID))
	{
		EM.ResetTimer(ShotTimerID, M.GetRandomFloat(10.25f, 15.75f));
		FireShot();
	}

	EM.ResetTimer(SeekTimerID, M.GetRandomFloat(15.25f, 20.5f));
}

void TheLander::SeekPersonMan()
{
	if (Y() < GroundCeilingY)
	{
		Velocity.y = M.GetRandomFloat(20.0f, 30.0f);
		GroundFloorY = M.GetRandomFloat(50.0f, (FieldSize.y * 0.5f) - 50.0f);
	}
	else if (Y() > GroundFloorY)
	{
		Velocity.y = M.GetRandomFloat(-30.0f, -20.0f);
		GroundCeilingY = M.GetRandomFloat(50.0f, 250.0f);
	}

	if (EM.TimerElapsed(SeekTimerID))
	{
		for (int i = 0; i < 10; i++)
		{
			if (People[i] != nullptr && People[i]->Enabled &&
				People[i]->State != ThePerson::TargetedByLander &&
				People[i]->State == ThePerson::OnGround)
			{
				if (People[i]->X() < X() + 55.0f && People[i]->X() > X() - 55.0f)
				{
					if (GetRandomValue(0, SeekTimerChance) != 0)
					{
						EM.ResetTimer(SeekTimerID,
							M.GetRandomFloat(SeekTimerAmountMin, SeekTimerAmountMax));
						return;
					}

					PersonTargetID = i;
					State = LanderStateList::FoundPersonMan;
					Velocity.x = 0.0f;
					Velocity.y = M.GetRandomFloat(30.0f, 40.0f);
					People[i]->State = ThePerson::TargetedByLander;
				}
			}
		}
	}

	if (EM.TimerElapsed(ShotTimerID))
	{
		EM.ResetTimer(ShotTimerID, M.GetRandomFloat(5.15f, 8.75f));
		FireShot();
	}
}

void TheLander::GoingDown()
{
	if (X() != People[PersonTargetID]->X())
	{
		float distanceX = People[PersonTargetID]->X() - X();
		Velocity.x = distanceX * 2.5f;
	}

	if (Vector3Distance(People[PersonTargetID]->Position, Position) < 25.0f)
	{
		State = LanderStateList::TakePersonMan;
		PlaySound(PersonGrabbedSound);
		Velocity.y = M.GetRandomFloat(-60.0f, -40.0f);
		Velocity.x = 0.0f;
	}

	if (EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
		EM.ResetTimer(ShotTimerID, M.GetRandomFloat(0.5f, 1.5f));
	}
}

void TheLander::GrabPersonMan()
{
	People[PersonTargetID]->Y(Y() + 25.0f);

	if (Y() < -FieldSize.y * 0.333f)
	{
		State = LanderStateList::Mutate;
		PlaySound(MutateSound);
		Velocity.y = 0.0f;
		People[PersonTargetID]->Velocity.y = -60.0f;
	}

	if (EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
		EM.ResetTimer(ShotTimerID, M.GetRandomFloat(0.25f, 1.0f));
	}
}

void TheLander::SpawnMutant()
{
	if (People[PersonTargetID]->Y() < Y())
	{
		People[PersonTargetID]->Destroy();
		People[PersonTargetID]->Velocity.y = 0.0f;
		MutateLander = true;
		Player->EnemyUpdate = true;
	}
}

void TheLander::Destroy()
{
	Enemy::Destroy();

	MutateLander = false;
	State = LanderStateList::LoweringToSeek;
}

bool TheLander::CheckCollision()
{
	Enemy::CheckCollision();

	return false;
}