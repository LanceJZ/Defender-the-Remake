#pragma once
#include "Globals.h"
#include "Common.h"
#include "Utilities.h"
#include "TheBackground.h"
#include "ThePlayer.h"
#include "EnemyControl.h"
#include "ThePerson.h"

enum GameState
{
	PlayerHitByEnemy,
	AfterPlayerHit,
	WaveStart,
	NewWave,
	GameOver,
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

	GameState State = MainMenu;

	void SetBackground(TheBackground* background);
	void SetPlayer(ThePlayer* player);
	void SetEnemies(EnemyControl* enemies);
	void SetPersonModel(Model model);
	void SetPersonRadarModel(Model model);
	void SetSmartBombModel(Model model);

	void SetPersonGrabbedSound(Sound sound);
	void SetPersonDroppedSound(Sound sound);
	void SetPersonCaughtSound(Sound sound);
	void SetPersonLandedSound(Sound sound);
	void SetPersonSplatSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Input();
	void Update();
	void Draw2D();

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


	Sound PersonGrabbedSound = { 0 };
	Sound PersonDroppedSound = { 0 };
	Sound PersonCaughtSound = { 0 };
	Sound PersonLandedSound = { 0 };
	Sound PersonSplatSound = { 0 };

	Model PersonModel = { 0 };
	Model PersonRadarModel = { 0 };
	Model SmartBombModel = { 0 };

	std::vector<Model3D*> PlayerLives = {};
	std::vector<Model3D*> SmartbombIcons = {};

	ThePlayer* Player = {};
	EnemyControl* Enemies = {};
	TheBackground* BackGround = {};
	ThePerson* People[10] = { nullptr };

	void UpdatePersonMan();
	void SpawnPersonMan(int count);
	void SetupPersonMan();
	void PlayerHitReset();
	void GameInPlay();
	void WaveStarting();
	void ResetField();
	void LivesDisplay();
	void LivesDisplayUpdate();
	void SmartbombsDisplay();
	void SmartbombsDisplayUpdate();
	void NewGame();
};

