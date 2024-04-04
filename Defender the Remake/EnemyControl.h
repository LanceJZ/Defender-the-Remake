#pragma once
#include "Globals.h"
#include "Common.h"
#include "ThePlayer.h"
#include "ThePerson.h"
#include "TheLander.h"
#include "TheMutant.h"
#include "TheBomber.h"
#include "TheSwarmer.h"
#include "ThePod.h"
#include "TheBaiter.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	std::vector<TheLander*> Landers = {};
	std::vector<TheMutant*> Mutants = {};
	std::vector<TheBomber*> Bombers = {};
	std::vector<TheSwarmer*> Swarmers = {};
	std::vector<ThePod*> Pods = {};
	std::vector<TheBaiter*> Baiters = {};

	bool LandersTurnedToMutants = false;

	void SetPlayer(ThePlayer* player);
	void SetPeople(ThePerson* people[10]);
	void SetLanderModel(Model model);
	void SetMutantModel(Model model);
	void SetBomberModel(Model model);
	void SetSwarmerModel(Model model);
	void SetPodModel(Model model);
	void SetBaiterModel(Model model);
	void SetShotModel(Model model);
	void SetBombModel(Model model);
	void SetLanderRadarModel(Model model);
	void SetMutantRadarModel(Model model);
	void SetBomberRadarModel(Model model);
	void SetSwarmerRadarModel(Model model);
	void SetPodRadarModel(Model model);
	void SetBaiterRadarModel(Model model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

	void AllDead();
	void NewGame();

private:
	size_t SpawnTimerID = 0;

	int TotalSpawn = 10;
	int NumberSpawned = 0;
	int NumberMutants = 0;
	float SpawnTimerAmount = 10.0f;

	Vector2 AdjustedFieldSize = {};

	Model LanderModel = { 0 };
	Model MutantModel = { 0 };
	Model BomberModel = { 0 };
	Model SwarmerModel = { 0 };
	Model BaiterModel = { 0 };
	Model PodModel = { 0 };
	Model ShotModel = { 0 };
	Model BombModel = { 0 };
	Model RadarLanderModel = { 0 };
	Model RadarMutantModel = { 0 };
	Model RadarBomberModel = { 0 };
	Model RadarSwarmerModel = { 0 };
	Model RadarBaiterModel = { 0 };
	Model RadarPodModel = { 0 };

	ThePlayer* Player = nullptr;
	ThePerson* People[10] = {};

	void UpdateLander();
	void UpdateMutant();
	void UpdateBomber();
	void UpdateSwarmer();
	void UpdatePod();

	void SmartBomb();
	void PlayerHitReset();
	void Reset();

	void SpawnMoreLanders();
	void SpawnLanders(int count);
	void SpawnMutant(Vector3 position);
	void SpawnBomber(int count);
	void SpawnSwarmers(Vector3 position, int count);
	void SpawnPod(int count);
};
