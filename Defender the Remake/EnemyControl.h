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
	bool GameEnded = false;
	bool NoMorePeople = false;
	bool WaveEnded = false;
	bool TriggerLandChange = false;
	bool RestartWaveTriggered = false;

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

	void SetExplodeSound(Sound sound);
	void SetFireSound(Sound sound);
	void SetLanderMutateSound(Sound sound);
	void SetLandersSpawnSound(Sound sound);
	void SetMutantFireSound(Sound sound);
	void SetBaiterSpawnSound(Sound sound);
	void SetBomberExplodeSound(Sound sound);
	void SetPodExplodeSound(Sound sound);
	void SetSwarmerExplodeSound(Sound sound);
	void SetSwarmerFireSound(Sound sound);
	void SetPersonGrabbedSound(Sound sound);
	void SetPersonDroppedSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

	void StartNewWave();
	void AllDead();
	void NewGame();
	void ResetField();
	void RestartWave();
	void PlayerHitReset();
	void SmartBomb();

private:
	size_t SpawnTimerID = 0;

	bool BaiterTurnedOn = false;
	bool NoMoreLanders = false;
	bool NoMoreMutants = false;
	bool NoMoreBombers = false;
	bool NoMoreSwarmers = false;
	bool NoMorePods = false;

	int TotalSpawn = 0;
	int NumberSpawned = 0;
	int NumberMutants = 0;
	int NumberBombers = 0;
	int NumberSwarmers = 0;
	int NumberPods = 0;
	int Wave = 0;

	float SpawnTimerAmount = 10.0f;

	Vector2 AdjustedFieldSize = {};

	Sound ExplodeSound = { 0 };
	Sound FireSound = { 0 };
	Sound LanderMutateSound = { 0 };
	Sound LandersSpawnSound = { 0 };
	Sound MutantFireSound = { 0 };
	Sound BaiterSpawnSound = { 0 };
	Sound BomberExplodeSound = { 0 };
	Sound PodExplodeSound = { 0 };
	Sound SwarmerExplodeSound = { 0 };
	Sound SwarmerFireSound = { 0 };
	Sound PersonGrabbedSound = { 0 };
	Sound PersonDroppedSound = { 0 };

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

	void UpdateLanderStatus();
	void UpdateMutantStatus();
	void UpdateBomberStatus();
	void UpdateSwarmerStatus();
	void UpdatePodStatus();
	void UpdateBaiterStatus();

	void SpawnMoreLanders();
	void SpawnLanders(int count);
	void SpawnMutant(Vector3 position);
	void SpawnBomber(int count);
	void SpawnSwarmers(Vector3 position, int count);
	void SpawnPod(int count);
	void SpawnBaiter();
	void EndOfWave();
	void SmartBomb(float min, float max);
};
