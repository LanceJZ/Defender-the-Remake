#pragma once
#include "Globals.h"
#include "Common.h"
#include "Utilities.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "ThePerson.h"

enum GameState
{
	PlayerHitByEnemy,
	AfterPlayerHit,
	WaveStart,
	NewWave,
	GaveOver,
	InPlay,
	Pause,
	HighScores,
	MainMenu
};

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

	void EndOfWave();

private:
	size_t NewWaveTimerID = 0;
	size_t PlayerDeathTimerID = 0;
	size_t PlayerResetTimerID = 0;
	size_t WaveStartTimerID = 0;
	size_t PeopleIDs[10] = { 0 };

	int Wave = 0;
	int NumberOfPeopleAlive = 0;

	Vector2 AdjustedFieldSize = {};

	GameState State = MainMenu;

	Model PersonModel = { 0 };
	Model PersonRadarModel = { 0 };
	ThePlayer* Player = {};
	EnemyControl* Enemies = {};
	ThePerson* People[10] = { nullptr };

	void UpdatePersonMan();
	void SpawnPersonMan(int count);
	void SetupPersonMan();
	void PlayerHitReset();
	void ResetField();
};

