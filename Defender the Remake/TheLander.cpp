#include "TheLander.h"

TheLander::TheLander()
{
	SeekTimerID = TheManagers.EM.AddTimer();
}

TheLander::~TheLander()
{
}

void TheLander::SetPeople(ThePerson* people[10])
{
	for (int i = 0; i < 10; i++)
	{
		People[i] = people[i];
	}
}

bool TheLander::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

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

	switch (State)
	{
	case StateList::LoweringToSeek:
		GoToSeek();
		break;
	case StateList::Seek:
		SeekPersonMan();
		break;
	case StateList::FoundPersonMan:
		GoingDown();
		break;
	case StateList::TakePersonMan:
		GrabPersonMan();
		break;
	case StateList::Mutate:
		SpawnMutant();
		break;
	}
}

void TheLander::Draw()
{
	Model3D::Draw();

}

void TheLander::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

	Position.x = GetRandomFloat(-FieldSize.x * 0.5f, FieldSize.x * 0.5f);
	Position.y = -GetScreenHeight() * 0.333f;

	State = StateList::LoweringToSeek;

	float xSpeed = 50.0f;

	if (GetRandomValue(0, 1) == 0)
	{
		Velocity.x = GetRandomFloat(xSpeed, xSpeed * 2.0f);
	}
	else
	{
		Velocity.x = GetRandomFloat(-xSpeed * 2.0f, -xSpeed);
	}

	Velocity.y = GetRandomFloat(20.0f, 30.0f);
	GroundFloorY = GetRandomFloat(20.0f, 100.0f);

	SeekTimerAmountMin = 15.25f;
	SeekTimerAmountMax = 35.5f;
	SeekTimerChance = 10;
}

void TheLander::Reset()
{
	Enemy::Reset();



	Destroy();
}

void TheLander::Hit()
{
}

void TheLander::GoToSeek()
{
	if (Y() > (GetScreenHeight() * 0.2f) + GroundFloorY)
	{
		State = StateList::Seek;
	}

	if (TheManagers.EM.TimerElapsed(ShotTimerID))
	{
		TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(2.75f, 4.75f));
		FireShot();
	}

	TheManagers.EM.ResetTimer(SeekTimerID, GetRandomFloat(5.25f, 20.5f));
}

void TheLander::SeekPersonMan()
{
	if (Y() < GroundCeilingY)
	{
		Velocity.y = GetRandomFloat(20.0f, 30.0f);
		GroundFloorY = GetRandomFloat(50.0f, (FieldSize.y * 0.5f) - 50.0f);
	}
	else if (Y() > GroundFloorY)
	{
		Velocity.y = GetRandomFloat(-30.0f, -20.0f);
		GroundCeilingY = GetRandomFloat(50.0f, 250.0f);
	}

	if (TheManagers.EM.TimerElapsed(SeekTimerID))
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
						TheManagers.EM.ResetTimer(SeekTimerID,
							GetRandomFloat(SeekTimerAmountMin, SeekTimerAmountMax));
						return;
					}

					PersonTargetID = i;
					State = StateList::FoundPersonMan;
					Velocity.x = 0.0f;
					Velocity.y = GetRandomFloat(30.0f, 40.0f);
					People[i]->State = ThePerson::TargetedByLander;
				}
			}
		}
	}

	if (TheManagers.EM.TimerElapsed(ShotTimerID))
	{
		TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(2.0f, 3.5f));
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

	if (Y() + 25 > People[PersonTargetID]->Y() && Y() - 25 < People[PersonTargetID]->Y())
	{
		State = StateList::TakePersonMan;
		Velocity.y = GetRandomFloat(-60.0f, -40.0f);
		Velocity.x = 0.0f;
	}

	if (TheManagers.EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
		TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.5f, 1.5f));
	}
}

void TheLander::GrabPersonMan()
{
	People[PersonTargetID]->Y(Y() + 25.0f);

	if (Y() < -FieldSize.y * 0.333f)
	{
		State = StateList::Mutate;
		Velocity.y = 0.0f;
		People[PersonTargetID]->Velocity.y = -60.0f;
	}

	if (TheManagers.EM.TimerElapsed(ShotTimerID))
	{
		FireShot();
		TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(0.25f, 1.0f));
	}
}

void TheLander::SpawnMutant()
{
	if (People[PersonTargetID]->Y() < Y())
	{
		People[PersonTargetID]->Destroy();
		People[PersonTargetID]->Velocity.y = 0.0f;
		MutateLander = true;
	}
}

bool TheLander::CheckCollision()
{
	return false;
}

void TheLander::Destroy()
{
	Enemy::Destroy();

	MutateLander = false;
	State = StateList::LoweringToSeek;
}
