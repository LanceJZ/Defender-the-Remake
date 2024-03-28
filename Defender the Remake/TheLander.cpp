#include "TheLander.h"

TheLander::TheLander()
{
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
		CheckPlayfieldSidesWarp(4.0f, 3.0f);
		break;
	case StateList::Seek:
		SeekPersonMan();
		CheckPlayfieldSidesWarp(4.0f, 3.0f);
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

	State = StateList::LoweringToSeek;

	float velX = 0.0f;

	if (GetRandomValue(0, 1) == 0)
	{
		velX = GetRandomFloat(30.0f, 60.0f);
	}
	else
	{
		velX = GetRandomFloat(-60.0f, -30.0f);
	}

	float velY = GetRandomFloat(20.0f, 30.0f);
	Velocity = { velX, velY, 0.0f };
	GroundHoverY = GetRandomFloat(-80.0f, 20.0f);
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
	if (Y() > (GetScreenHeight() * 0.2f) + GroundHoverY)
	{
		State = StateList::Seek;
		Velocity.y = 0.0f;
	}

	if (TheManagers.EM.TimerElapsed(ShotTimerID))
	{
		TheManagers.EM.ResetTimer(ShotTimerID, GetRandomFloat(2.75f, 4.75f));
		FireShot();
	}
}

void TheLander::SeekPersonMan()
{
	for (int i = 0; i < 10; i++)
	{
		if (People[i] != nullptr && People[i]->Enabled &&
			People[i]->State != ThePerson::TargetedByLander &&
			People[i]->State == ThePerson::OnGround)
		{
			if (People[i]->X() < X() + 25.0f && People[i]->X() > X() - 25.0f)
			{
				PersonTargetID = i;
				State = StateList::FoundPersonMan;
				Velocity.x = 0.0f;
				Velocity.y = GetRandomFloat(30.0f, 40.0f);
				People[i]->State = ThePerson::TargetedByLander;
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
		if (X() < People[PersonTargetID]->X())
		{
			Velocity.x = GetRandomFloat(5.0f, 15.0f);
		}
		else
		{
			Velocity.x = GetRandomFloat(-15.0f, -5.0f);
		}
	}
	else
	{
		Velocity.x = 0.0f;
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
		People[PersonTargetID]->Enabled = false;
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
}
