#include "EnemyControl.h"

EnemyControl::EnemyControl()
{
	SpawnTimerID = EM.AddTimer(SpawnTimerAmount);
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

	return true;
}

void EnemyControl::Update()
{
	Common::Update();

	if (EM.TimerElapsed(SpawnTimerID))
	{
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

	if (NoMoreLanders && NoMoreMutants && NoMoreBombers && NoMoreSwarmers &&
		NoMorePods && !GameEnded && !RestartWaveTriggered) EndOfWave();
}

void EnemyControl::StartNewWave()
{
	WaveEnded = false;

	if (TotalSpawn < 30) TotalSpawn += 5;

	NumberSpawned = 0;
	Wave++;
	EM.ResetTimer(SpawnTimerID, 1.0f);

	SpawnBomber(Wave);
	SpawnPod(Wave);

	Player->Reset();
}

void EnemyControl::AllDead()
{
}

void EnemyControl::NewGame()
{
	TotalSpawn = 10;
	NumberSpawned = 0;
	Wave = 0;
	EM.ResetTimer(SpawnTimerID, 1.0f);
	GameEnded = false;
	ResetField();
	SpawnMoreLanders();
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

	Particles.ResetCubes();
}

void EnemyControl::RestartWave()
{
	EM.ResetTimer(SpawnTimerID, 1.0f);
	SpawnBomber(NumberBombers);
	SpawnPod(NumberPods);

	Vector3 position = { GameWindowHalfWidth * 4, -GameWindowHalfHeight, 0.0f };

	SpawnSwarmers(position, NumberSwarmers);

	for (int i = 0; i < NumberMutants; i++)
	{
		SpawnMutant(position);
	}

}

void EnemyControl::PlayerHitReset()
{
	int numberSpawned = 0;

	for (const auto& lander : Landers)
	{
		if (lander->Enabled)
		{
			numberSpawned++;
		}
	}

	NumberSpawned -= numberSpawned;
	numberSpawned = 0;

	for (const auto& mutant : Mutants)
	{
		if (mutant->Enabled)
		{
			numberSpawned++;
		}
	}

	NumberMutants = numberSpawned;
	numberSpawned = 0;

	for (const auto& bomber : Bombers)
	{
		if (bomber->Enabled)
		{
			numberSpawned++;
		}
	}

	NumberBombers = numberSpawned;
	numberSpawned = 0;

	for (const auto& pod : Pods)
	{
		if (pod->Enabled)
		{
			numberSpawned++;
		}
	}

	NumberPods = numberSpawned;
	numberSpawned = 0;

	for (const auto& swarmer : Swarmers)
	{
		if (swarmer->Enabled)
		{
			numberSpawned++;
		}
	}

	NumberSwarmers = numberSpawned;

	ResetField();
}

void EnemyControl::SmartBomb()
{
	Player->SmartBombFired = false;
	float width = GetScreenWidth() / 1.4f;
	float x = Player->X();
	float max = x + width;
	float min = x - width;

	SmartBomb(min, max);

	if (max > GetScreenWidth() * 3.5f)
	{
		x = Player->X() - GetScreenWidth() * 7;
		max = x + width;
		min = x - width;
		SmartBomb(min, max);
	}
	else if (min < -GetScreenWidth() * 3.5f)
	{
		x = Player->X() + GetScreenWidth() * 7;
		max = x + width;
		min = x - width;
		SmartBomb(min, max);
	}
}

void EnemyControl::UpdateLanderStatus()
{
	if (NoMoreLanders) return;

	int spawnCounter = 0;

	if (NoMorePeople)
	{
		TriggerLandChange = true;

		for (const auto& lander : Landers)
		{
			if (lander->Enabled)
			{
				lander->MutateLander = true;
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
				lander->Destroy();
				SpawnMutant(lander->Position);
				break;
			}

			if (lander->BeenHit)
			{
				lander->Destroy();
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
			mutant->Destroy();
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
			bomber->Destroy();
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
			swarmer->Destroy();
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
	for (const auto& baiter : Baiters)
	{
		if (baiter->BeenHit)
		{
			baiter->Destroy();
			break;
		}
	}
}

void EnemyControl::SpawnLanders(int count)
{
	if (!Player->GameOver) PlaySound(LandersSpawnSound);

	for (int i = 0; i < count; i++)
	{
		bool spawnNew = true;
		size_t landerNumber = 0;
		size_t landerSpawnNumber = Landers.size();

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
			EM.AddModel3D(Landers[landerSpawnNumber], LanderModel);
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
	size_t mutantNumber = 0;
	size_t mutantSpawnNumber = Mutants.size();

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
		EM.AddModel3D(Mutants[mutantSpawnNumber], MutantModel);
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
		size_t bomberNumber = 0;
		size_t bomberSpawnNumber = Bombers.size();

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
			EM.AddModel3D(Bombers[bomberSpawnNumber], BomberModel);
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
		size_t swarmerNumber = 0;
		size_t swarmerSpawnNumber = Swarmers.size();

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
			EM.AddModel3D(Swarmers[swarmerSpawnNumber], SwarmerModel);
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
		size_t podNumber = 0;
		size_t podSpawnNumber = Pods.size();

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
			EM.AddModel3D(Pods[podSpawnNumber], PodModel);
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
	size_t baiterNumber = 0;
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
		EM.AddModel3D(Baiters[spawnNumber], BaiterModel);
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
	if (Particles.GetParticlesActive()) return;

	NoMoreBombers = false;
	NoMorePods = false;
	NoMoreSwarmers = false;
	NoMoreMutants = false;
	NoMoreLanders = false;
	WaveEnded = true;
}

void EnemyControl::SmartBomb(float min, float max)
{
	for (const auto& lander : Landers)
	{
		if (lander->Enabled)
		{
			if (lander->X() > min && lander->X() < max)
			{
				lander->Hit();
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
			}
		}
	}

	for (const auto& bomber : Bombers)
	{
		if (bomber->Enabled)
		{
			if (bomber->X() > min && bomber->X() < max)
			{
				bomber->Hit();
			}
		}
	}

	for (const auto& pod : Pods)
	{
		if (pod->Enabled)
		{
			if (pod->X() > min && pod->X() < max)
			{
				pod->Hit();
			}
		}
	}

	for (const auto& swarmer : Swarmers)
	{
		if (swarmer->Enabled)
		{
			if (swarmer->X() > min && swarmer->X() < max)
			{
				swarmer->Hit();
			}
		}
	}

	for (const auto& baiter : Baiters)
	{
		if (baiter->Enabled)
		{
			if (baiter->X() > min && baiter->X() < max)
			{
				baiter->Hit();
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
	EM.ResetTimer(SpawnTimerID, SpawnTimerAmount);
}