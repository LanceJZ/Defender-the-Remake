#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	SpawnTimerID = Managers.EM.AddTimer();
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

void EnemyControl::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
}

void EnemyControl::SetFireSound(Sound sound)
{
	FireSound = sound;
}

void EnemyControl::SetLanderMutateSound(Sound sound)
{
	LanderMutateSound = sound;
}

void EnemyControl::SetLandersSpawnSound(Sound sound)
{
	LandersSpawnSound = sound;
}

void EnemyControl::SetMutantFireSound(Sound sound)
{
	MutantFireSound = sound;
}

void EnemyControl::SetBaiterSpawnSound(Sound sound)
{
	BaiterSpawnSound = sound;
}

void EnemyControl::SetBomberExplodeSound(Sound sound)
{
	BomberExplodeSound = sound;
}

void EnemyControl::SetPodExplodeSound(Sound sound)
{
	PodExplodeSound = sound;
}

void EnemyControl::SetSwarmerExplodeSound(Sound sound)
{
	SwarmerExplodeSound = sound;
}

void EnemyControl::SetSwarmerFireSound(Sound sound)
{
	SwarmerFireSound = sound;
}

void EnemyControl::SetPersonGrabbedSound(Sound sound)
{
	PersonGrabbedSound = sound;
}

void EnemyControl::SetPersonDroppedSound(Sound sound)
{
	PersonDroppedSound = sound;
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

	if (Managers.EM.TimerElapsed(SpawnTimerID))
	{
		Managers.EM.ResetTimer(SpawnTimerID, SpawnTimerAmount);

		if (NumberSpawned < TotalSpawn)
		{
			SpawnMoreLanders();
		}
	}

	if (Player->EnemyUpdate)
	{
		Player->EnemyUpdate = false;
		UpdateLanderStatus();
		UpdateMutantStatus();
		UpdateBomberStatus();
		UpdatePodStatus();
		UpdateSwarmerStatus();
		UpdateBaiterStatus();
	}

	if (NoMoreLanders && NoMoreMutants && NoMoreBombers &&
		NoMoreSwarmers && NoMorePods && !GameEnded) EndOfWave();
}

void EnemyControl::StartNewWave()
{
	if (TotalSpawn < 30) TotalSpawn += 5;

	NumberSpawned = 0;
	Wave++;
	Managers.EM.ResetTimer(SpawnTimerID);

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
	for (const auto& lander : Landers)
	{
		for (const auto& shot : lander->Shots)
		{
			shot->Reset();
		}

		lander->Reset();
	}

	for (const auto& mutant : Mutants)
	{
		for (const auto& shot : mutant->Shots)
		{
			shot->Reset();
		}

		mutant->Reset();
	}

	for (const auto& swarmer : Swarmers)
	{
		for (const auto& shot : swarmer->Shots)
		{
			shot->Reset();
		}

		swarmer->Reset();
	}

	for (const auto& pod : Pods)
	{
		for (const auto& shot : pod->Shots)
		{
			shot->Reset();
		}

		pod->Reset();
	}

	for (const auto& bomber : Bombers)
	{
		for (auto shot : bomber->Shots)
		{
			shot->Reset();
		}

		bomber->Reset();
	}

	for (const auto& baiter : Baiters)
	{
		for (const auto& shot : baiter->Shots)
		{
			shot->Reset();
		}

		baiter->Reset();
	}
}

void EnemyControl::UpdateLanderStatus()
{
	if (NoMoreLanders) return;

	int spawnCounter = 0;

	if (NoMorePeople)
	{
		for (const auto& lander : Landers)
		{
			if (lander->Enabled)
			{
				lander->MutateLander = true;
				TriggerLandChange = true;
			}
		}

		return;
	}

	for (const auto& lander : Landers)
	{
		if (lander->Enabled)
		{
			spawnCounter++;

			if (lander->MutateLander)
			{
				lander->Reset();
				SpawnMutant(lander->Position);
				break;
			}

			if (lander->BeenHit)
			{
				lander->Reset();
				break;
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

void EnemyControl::UpdateMutantStatus()
{
	NoMoreMutants = true;

	for (const auto& mutant : Mutants)
	{
		if (mutant->Enabled) NoMoreMutants = false;

		if (mutant->BeenHit)
		{
			mutant->Reset();
			break;
		}
	}
}

void EnemyControl::UpdateBomberStatus()
{
	NoMoreBombers = true;

	for (const auto& bomber : Bombers)
	{
		if (bomber->Enabled) NoMoreBombers = false;

		if (bomber->BeenHit)
		{
			bomber->Reset();
			break;
		}
	}
}

void EnemyControl::UpdateSwarmerStatus()
{
	NoMoreSwarmers = true;

	for (const auto& swarmer : Swarmers)
	{
		if (swarmer->Enabled) NoMoreSwarmers = false;

		if (swarmer->BeenHit)
		{
			swarmer->Reset();
			break;
		}
	}
}

void EnemyControl::UpdatePodStatus()
{
	NoMorePods = true;

	for (const auto& pod : Pods)
	{
		if (pod->Enabled) NoMorePods = false;

		if (pod->BeenHit)
		{
			pod->Reset();
			SpawnSwarmers(pod->Position, 4);
			break;
		}
	}
}

void EnemyControl::UpdateBaiterStatus()
{

}

void EnemyControl::SmartBomb()
{
	Player->SmartBombFired = false;
	float width = GetScreenWidth() / 1.4f;
	float x = Player->X();
	float max = x + width;
	float min = x - width;

	for (const auto& lander : Landers)
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

	for (const auto& mutant : Mutants)
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
	Managers.EM.ResetTimer(SpawnTimerID);
}

void EnemyControl::SpawnLanders(int count)
{
	if (!Player->GameOver) PlaySound(LandersSpawnSound);

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
			Managers.EM.AddModel3D(Landers[landerSpawnNumber], LanderModel);
			Landers[landerSpawnNumber]->SetPlayer(Player);
			Landers[landerSpawnNumber]->SetPeople(People);
			Landers[landerSpawnNumber]->SetShotModel(ShotModel);
			Landers[landerSpawnNumber]->SetRadarModel(RadarLanderModel);
			Landers[landerSpawnNumber]->SetFireSound(FireSound);
			Landers[landerSpawnNumber]->SetExplodeSound(ExplodeSound);
			Landers[landerSpawnNumber]->SetPersonGrabbedSound(PersonGrabbedSound);
			Landers[landerSpawnNumber]->SetMutateSound(LanderMutateSound);
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
		Managers.EM.AddModel3D(Mutants[mutantSpawnNumber], MutantModel);
		Mutants[mutantSpawnNumber]->SetPlayer(Player);
		Mutants[mutantSpawnNumber]->SetShotModel(ShotModel);
		Mutants[mutantSpawnNumber]->SetRadarModel(RadarMutantModel);
		Mutants[mutantSpawnNumber]->SetFireSound(MutantFireSound);
		Mutants[mutantSpawnNumber]->SetExplodeSound(ExplodeSound);
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
			Managers.EM.AddModel3D(Bombers[bomberSpawnNumber], BomberModel);
			Bombers[bomberSpawnNumber]->SetPlayer(Player);
			Bombers[bomberSpawnNumber]->SetShotModel(BombModel);
			Bombers[bomberSpawnNumber]->SetRadarModel(RadarBomberModel);
			Bombers[bomberSpawnNumber]->SetExplodeSound(BomberExplodeSound);
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
			Managers.EM.AddModel3D(Swarmers[swarmerSpawnNumber], SwarmerModel);
			Swarmers[swarmerSpawnNumber]->SetPlayer(Player);
			Swarmers[swarmerSpawnNumber]->SetShotModel(ShotModel);
			Swarmers[swarmerSpawnNumber]->SetRadarModel(RadarSwarmerModel);
			Swarmers[swarmerSpawnNumber]->SetExplodeSound(SwarmerExplodeSound);
			Swarmers[swarmerSpawnNumber]->SetFireSound(SwarmerFireSound);
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
			Managers.EM.AddModel3D(Pods[podSpawnNumber], PodModel);
			Pods[podSpawnNumber]->SetPlayer(Player);
			Pods[podSpawnNumber]->SetShotModel(ShotModel);
			Pods[podSpawnNumber]->SetRadarModel(RadarPodModel);
			Pods[podSpawnNumber]->SetExplodeSound(PodExplodeSound);
		}

		Pods[podSpawnNumber]->Spawn({ 0.0f });
	}
}

void EnemyControl::SpawnBaiter()
{
	if (!Player->GameOver) PlaySound(BaiterSpawnSound);

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
		Managers.EM.AddModel3D(Baiters[spawnNumber], BaiterModel);
		Baiters[spawnNumber]->SetPlayer(Player);
		Baiters[spawnNumber]->SetShotModel(ShotModel);
		Baiters[spawnNumber]->SetRadarModel(RadarBaiterModel);
		Baiters[spawnNumber]->SetExplodeSound(ExplodeSound);
		Baiters[spawnNumber]->SetFireSound(FireSound);
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
