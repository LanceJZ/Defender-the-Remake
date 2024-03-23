#include "EnemyControl.h"

EnemyControl::EnemyControl()
{

}

EnemyControl::~EnemyControl()
{
}

void EnemyControl::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void EnemyControl::SetPeople(ThePerson* people[10])
{
	for (int i = 0; i < 10; i++)
	{
		People[i] = people[i];
	}
}

void EnemyControl::SetLanderModel(Model model)
{
	LanderModel = model;
}

void EnemyControl::SetMutantModel(Model model)
{
	MutantModel = model;
}

void EnemyControl::SetShotModel(Model model)
{
	ShotModel = model;
}

bool EnemyControl::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

	return false;
}

bool EnemyControl::BeginRun()
{

	return false;
}

void EnemyControl::Update()
{
	Common::Update();

	UpdateLander();
	UpdateMutant();

	if (Player->BeenHit)
	{
		PlayerHitReset();
	}

	if (Player->SmartBombFired)
	{
		SmartBomb();
	}

}

void EnemyControl::AllDead()
{
}

void EnemyControl::NewGame()
{
}

void EnemyControl::UpdateLander()
{
}

void EnemyControl::UpdateMutant()
{
}

void EnemyControl::SmartBomb()
{
	Player->SmartBombFired = false;
	float width = GetScreenWidth() / 1.4f;
	float x = Player->X();
	float max = x + width;
	float min = x - width;

	for (auto lander : Landers)
	{
		if (lander->Enabled)
		{
			if (lander->X() > min && lander->X() < max)
			{
				lander->Hit();
				lander->Reset();
			}
		}
	}

	for (auto mutant : Mutants)
	{
		if (mutant->Enabled)
		{
			if (mutant->X() > min && mutant->X() < max)
			{
				mutant->Hit();
				mutant->Reset();
			}
		}
	}
}

void EnemyControl::PlayerHitReset()
{
	Reset();
	Player->BeenHit = false;
}

void EnemyControl::Reset()
{
}