#pragma once
#include "Globals.h"
#include "Common.h"
#include "ThePlayer.h"
#include "ThePerson.h"
#include "TheLander.h"
#include "TheMutant.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	std::vector<TheLander*> Landers = {};
	std::vector<TheMutant*> Mutants = {};

	bool LandersTurnedToMutants = false;

	void SetPlayer(ThePlayer* player);
	void SetPeople(ThePerson* people[10]);
	void SetLanderModel(Model model);
	void SetMutantModel(Model model);
	void SetShotModel(Model model);
	void SetLanderRadarModel(Model model);
	void SetMutantRadarModel(Model model);

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
	Model ShotModel = { 0 };
	Model RadarLanderModel = { 0 };
	Model RadarMutantModel = { 0 };

	ThePlayer* Player = nullptr;
	ThePerson* People[10] = {};

	void UpdateLander();
	void UpdateMutant();

	void SmartBomb();
	void PlayerHitReset();
	void Reset();

	void SpawnMoreLanders();
	void SpawnLanders(int count);
	void SpawnMutant(Vector3 position);
};

