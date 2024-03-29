#pragma once
#include "Globals.h"
#include "Common.h"
#include "Utilities.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "ThePerson.h"

class GameLogic : public Common
{
public:
	GameLogic();
	virtual ~GameLogic();


	void SetPlayer(ThePlayer* player);
	void SetEnemies(EnemyControl* enemies);
	void SetPersonModel(Model model);
	void SetPersonRadarModel(Model model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

private:
	size_t PeopleIDs[10] = { 0 };
	Vector2 AdjustedFieldSize = {};

	Model PersonModel = { 0 };
	Model PersonRadarModel = { 0 };
	ThePlayer* Player = {};
	EnemyControl* Enemies = {};
	ThePerson* People[10] = { nullptr };

	void UpdatePersonMan();
	void SpawnPersonMan(int count);
	void SetupPersonMan();
};

