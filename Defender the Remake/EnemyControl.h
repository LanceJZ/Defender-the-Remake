#pragma once
#include "Globals.h"
#include "Common.h"
#include "ThePlayer.h"
#include "TheLander.h"
#include "TheMutant.h"
#include "ThePerson.h"

class EnemyControl : public Common
{
public:
	EnemyControl();
	virtual ~EnemyControl();

	std::vector<TheLander*> Landers = {};
	std::vector<TheMutant*> Mutants = {};
	ThePerson* People[10] = { nullptr };

	bool LandersTurnedToMutants = false;

	void SetPlayer(ThePlayer* player);
	void SetLanderModel(Model model);
	void SetMutantModel(Model model);
	void SetShotModel(Model model);
	void SetPersonModel(Model model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

	void NewGame();

private:
	Model LanderModel = { 0 };
	Model MutantModel = { 0 };
	Model ShotModel = { 0 };
	Model PersonModel = { 0 };

	ThePlayer *Player = nullptr;

	void UpdateLander();
	void UpdateMutant();
	void UpdatePerson();

	void SmartBomb();
	void PlayerHitReset();
	void Reset();
	void AllDead();
};

