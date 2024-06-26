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

void EnemyControl::SetBomberModel(Model model)
{
	BomberModel = model;
}

void EnemyControl::SetSwarmerModel(Model model)
{
	SwarmerModel = model;
}

void EnemyControl::SetPodModel(Model model)
{
	PodModel = model;
}

void EnemyControl::SetBaiterModel(Model model)
{
	BaiterModel = model;
}

void EnemyControl::SetShotModel(Model model)
{
	ShotModel = model;
}

void EnemyControl::SetBombModel(Model model)
{
	BombModel = model;
}

void EnemyControl::SetLanderRadarModel(Model model)
{
	RadarLanderModel = model;
}

void EnemyControl::SetMutantRadarModel(Model model)
{
	RadarMutantModel = model;
}

void EnemyControl::SetBomberRadarModel(Model model)
{
	RadarBomberModel = model;
}

void EnemyControl::SetSwarmerRadarModel(Model model)
{
	RadarSwarmerModel = model;
}

void EnemyControl::SetPodRadarModel(Model model)
{
	RadarPodModel = model;
}

void EnemyControl::SetBaiterRadarModel(Model model)
{
	RadarBaiterModel = model;
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

	ResetField();

	TotalSpawn = 10;

	return true;
}

void EnemyControl::Update()
{
	Common::Update();

	if (TheManagers.EM.TimerElapsed(SpawnTimerID))
	{
		TheManagers.EM.ResetTimer(SpawnTimerID, SpawnTimerAmount);

		if (NumberSpawned < TotalSpawn)
		{
			SpawnMoreLanders();
		}
	}

	if (Player->EnemyUpdate)
	{
		Player->EnemyUpdate = false;
		UpdateLander();
		UpdateMutant();
		UpdateBomber();
		UpdatePod();
		UpdateSwarmer();
		UpdateBaiter();
	}

	if (NoMoreLanders && NoMoreMutants && NoMoreBombers &&
		NoMoreSwarmers && NoMorePods && !GameEnded) EndOfWave();
}

void EnemyControl::StartNewWave()
{
	if (TotalSpawn < 30) TotalSpawn += 5;

	NumberSpawned = 0;
	Wave++;
	TheManagers.EM.ResetTimer(SpawnTimerID);

	SpawnBomber(Wave);
	SpawnPod(Wave);

	Player->Reset();
}

void EnemyControl::AllDead()
{
}

void EnemyControl::NewGame()
{
}

void EnemyControl::ResetField()
{
	for (auto lander : Landers)
	{
		for (auto shot : lander->Shots)
		{
			shot->Reset();
		}

		lander->Reset();
	}

	for (auto mutant : Mutants)
	{
		for (auto shot : mutant->Shots)
		{
			shot->Reset();
		}

		mutant->Reset();
	}

	for (auto swarmer : Swarmers)
	{
		for (auto shot : swarmer->Shots)
		{
			shot->Reset();
		}

		swarmer->Reset();
	}

	for (auto pod : Pods)
	{
		for (auto shot : pod->Shots)
		{
			shot->Reset();
		}

		pod->Reset();
	}

	for (auto bomber : Bombers)
	{
		for (auto shot : bomber->Shots)
		{
			shot->Reset();
		}

		bomber->Reset();
	}

	for (auto baiter : Baiters)
	{
		for (auto shot : baiter->Shots)
		{
			shot->Reset();
		}

		baiter->Reset();
	}

	TheManagers.PM.Reset();
}

void EnemyControl::UpdateLander()
{
	if (NoMoreLanders) return;

	int spawnCounter = 0;

	if (NoMorePeople)
	{
		for (auto lander : Landers)
		{
			if (lander->Enabled)
			{
				lander->MutateLander = true;
				TriggerLandChange = true;
			}
		}
	}

	for (auto lander : Landers)
	{
		if (lander->Enabled)
		{
			spawnCounter++;

			if (lander->MutateLander)
			{
				lander->Reset();
				SpawnMutant(lander->Position);
			}
		}
	}

	if (spawnCounter == 0 && NumberSpawned < TotalSpawn)
	{
		SpawnMoreLanders();
		return;
	}

	if (spawnCounter < 3 && NumberSpawned == TotalSpawn) BaiterTurnedOn = true;

	if (spawnCounter == 0 && NumberSpawned == TotalSpawn) NoMoreLanders = true;
}

void EnemyControl::UpdateMutant()
{
	NoMoreMutants = false;

	for (auto mutant : Mutants)
	{
		if (mutant->Enabled) return;
	}

	NoMoreMutants = true;
}

void EnemyControl::UpdateBomber()
{
	NoMoreBombers = false;

	for (auto bomber : Bombers)
	{
		if (bomber->Enabled) return;
	}

	NoMoreBombers = true;
}

void EnemyControl::UpdateSwarmer()
{
	NoMoreSwarmers = false;

	for (auto swarmer : Swarmers)
	{
		if (swarmer->Enabled) return;
	}

	NoMoreSwarmers = true;
}

void EnemyControl::UpdatePod()
{
	NoMorePods = true;

	for (auto pod : Pods)
	{
		if (!pod->Enabled)
		{
			if (pod->BeenHit)
			{
				pod->Reset();
				SpawnSwarmers(pod->Position, 4);
				break;
			}
		}

		if (pod->Enabled) NoMorePods = false;
	}
}

void EnemyControl::UpdateBaiter()
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

		Landers[landerSpawnNumber]->Spawn({ 0.0f });
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

		mutantNumber++;
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

void EnemyControl::SpawnBomber(int count)
{
	for (int i = 0; i < count; i++)
	{
		bool spawnNew = true;
		int bomberNumber = 0;
		int bomberSpawnNumber = (int)Bombers.size();

		for (const auto& bomber : Bombers)
		{
			if (!bomber->Enabled)
			{
				spawnNew = false;
				bomberSpawnNumber = bomberNumber;
				break;
			}

			bomberNumber++;
		}

		if (spawnNew)
		{
			Bombers.push_back(DBG_NEW TheBomber());
			TheManagers.EM.AddModel3D(Bombers[bomberSpawnNumber], BomberModel);
			Bombers[bomberSpawnNumber]->SetPlayer(Player);
			Bombers[bomberSpawnNumber]->SetShotModel(BombModel);
			Bombers[bomberSpawnNumber]->SetRadarModel(RadarBomberModel);
			Bombers[bomberSpawnNumber]->Initialize(TheUtilities);
			Bombers[bomberSpawnNumber]->BeginRun();
		}

		Bombers[bomberSpawnNumber]->Spawn({ 0.0f });
	}


}

void EnemyControl::SpawnSwarmers(Vector3 position, int count)
{
	for (int i = 0; i < count; i++)
	{
		bool spawnNew = true;
		int swarmerNumber = 0;
		int swarmerSpawnNumber = (int)Swarmers.size();

		for (const auto& swarmer : Swarmers)
		{
			if (!swarmer->Enabled)
			{
				spawnNew = false;
				swarmerSpawnNumber = swarmerNumber;
				break;
			}

			swarmerNumber++;
		}

		if (spawnNew)
		{
			Swarmers.push_back(DBG_NEW TheSwarmer());
			TheManagers.EM.AddModel3D(Swarmers[swarmerSpawnNumber], SwarmerModel);
			Swarmers[swarmerSpawnNumber]->SetPlayer(Player);
			Swarmers[swarmerSpawnNumber]->SetShotModel(ShotModel);
			Swarmers[swarmerSpawnNumber]->SetRadarModel(RadarSwarmerModel);
			Swarmers[swarmerSpawnNumber]->Initialize(TheUtilities);
			Swarmers[swarmerSpawnNumber]->BeginRun();
		}

		Swarmers[swarmerSpawnNumber]->Spawn(position);
	}
}

void EnemyControl::SpawnPod(int count)
{
	for (int i = 0; i < count; i++)
	{
		bool spawnNew = true;
		int podNumber = 0;
		int podSpawnNumber = (int)Pods.size();

		for (const auto& pod : Pods)
		{
			if (!pod->Enabled)
			{
				spawnNew = false;
				podSpawnNumber = podNumber;
				break;
			}

			podNumber++;
		}

		if (spawnNew)
		{
			Pods.push_back(DBG_NEW ThePod());
			TheManagers.EM.AddModel3D(Pods[podSpawnNumber], PodModel);
			Pods[podSpawnNumber]->SetPlayer(Player);
			Pods[podSpawnNumber]->SetShotModel(ShotModel);
			Pods[podSpawnNumber]->SetRadarModel(RadarPodModel);
			Pods[podSpawnNumber]->Initialize(TheUtilities);
			Pods[podSpawnNumber]->BeginRun();
		}

		Pods[podSpawnNumber]->Spawn({ 0.0f });
	}
}

void EnemyControl::SpawnBaiter()
{
	size_t spawnNumber = Baiters.size();
	int baiterNumber = 0;
	bool spawnNew = true;

	for (const auto& baiter : Baiters)
	{
		if (!baiter->Enabled)
		{
			spawnNumber = baiterNumber;
			spawnNew = false;
			break;
		}

		baiterNumber++;
	}

	if (spawnNew)
	{
		Baiters.push_back(DBG_NEW TheBaiter());
		TheManagers.EM.AddModel3D(Baiters[spawnNumber], BaiterModel);
		Baiters[spawnNumber]->SetPlayer(Player);
		Baiters[spawnNumber]->SetShotModel(ShotModel);
		Baiters[spawnNumber]->SetRadarModel(RadarBaiterModel);
		Baiters[spawnNumber]->Initialize(TheUtilities);
		Baiters[spawnNumber]->BeginRun();
	}

	Baiters[spawnNumber]->Spawn({ 0.0f });
}

void EnemyControl::EndOfWave()
{
	NoMoreBombers = false;
	NoMorePods = false;
	NoMoreSwarmers = false;
	NoMoreMutants = false;
	NoMoreLanders = false;
	WaveEnded = true;

	ResetField();
	StartNewWave();
}
