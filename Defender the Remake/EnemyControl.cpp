#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	SpawnTimerID = TheManagers.EM.AddTimer();
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

void EnemyControl::SetLanderRadarModel(Model model)
{
	RadarLanderModel = model;
}

void EnemyControl::SetMutantRadarModel(Model model)
{
	RadarMutantModel = model;
}

bool EnemyControl::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	return true;
}

bool EnemyControl::BeginRun()
{
	Common::BeginRun();

	Reset();

	return true;
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
	if (TheManagers.EM.TimerElapsed(SpawnTimerID))
	{
		TheManagers.EM.ResetTimer(SpawnTimerID, SpawnTimerAmount);

		if (NumberSpawned < TotalSpawn)
		{
			SpawnMoreLanders();
		}
	}

	for (auto lander : Landers)
	{
		if (lander->Enabled)
		{
			if (lander->MutateLander)
			{
				lander->Reset();
				SpawnMutant(lander->Position);
				break;
			}
		}
	}
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
	TheManagers.EM.ResetTimer(SpawnTimerID, 2.0f);
}

void EnemyControl::SpawnMoreLanders()
{
	int spawn = 5;

	if (NumberSpawned + spawn > TotalSpawn)
	{
		spawn = TotalSpawn - NumberSpawned;
	}

	NumberSpawned += spawn;
	SpawnLanders(spawn);
	TheManagers.EM.ResetTimer(SpawnTimerID);
}

void EnemyControl::SpawnLanders(int count)
{
	for (int i = 0; i < count; i++)
	{
		bool spawnNew = true;
		int landerNumber = 0;
		int landerSpawnNumber = (int)Landers.size();

		for (const auto& lander : Landers)
		{
			if (!lander->Enabled)
			{
				spawnNew = false;
				landerSpawnNumber = landerNumber;
				break;
			}

			landerNumber++;
		}

		if (spawnNew)
		{
			Landers.push_back(DBG_NEW TheLander());
			TheManagers.EM.AddModel3D(Landers[landerSpawnNumber], LanderModel);
			Landers[landerSpawnNumber]->SetPlayer(Player);
			Landers[landerSpawnNumber]->SetPeople(People);
			Landers[landerSpawnNumber]->SetShotModel(ShotModel);
			Landers[landerSpawnNumber]->SetRadarModel(RadarLanderModel);
			Landers[landerSpawnNumber]->Initialize(TheUtilities);
			Landers[landerSpawnNumber]->BeginRun();
		}

		float x = GetRandomFloat(-AdjustedFieldSize.x, AdjustedFieldSize.x);
		float y = -GetScreenHeight() * 0.333f;
		Landers[landerSpawnNumber]->Spawn({x, y, 0.0f});
	}
}

void EnemyControl::SpawnMutant(Vector3 position)
{
	bool spawnNew = true;
	int mutantNumber = 0;
	int mutantSpawnNumber = (int)Mutants.size();

	for (const auto& mutant : Mutants)
	{
		if (!mutant->Enabled)
		{
			spawnNew = false;
			mutantSpawnNumber = mutantNumber;
			break;
		}
	}

	if (spawnNew)
	{
		Mutants.push_back(DBG_NEW TheMutant());
		TheManagers.EM.AddModel3D(Mutants[mutantSpawnNumber], MutantModel);
		Mutants[mutantSpawnNumber]->SetPlayer(Player);
		Mutants[mutantSpawnNumber]->SetShotModel(ShotModel);
		Mutants[mutantSpawnNumber]->SetRadarModel(RadarMutantModel);
		Mutants[mutantSpawnNumber]->Initialize(TheUtilities);
		Mutants[mutantSpawnNumber]->BeginRun();
	}

	Mutants[mutantSpawnNumber]->Spawn(position);
}
