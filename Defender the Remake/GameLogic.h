#pragma once
#include "Globals.h"
#include "Common.h"
#include "Utilities.h"
#include "ThePlayer.h"
#include "EnemyControl.h"

class GameLogic : public Common
{
public:
	GameLogic();
	virtual ~GameLogic();

	void SetPlayer(ThePlayer* player);
	void SetEnemies(EnemyControl* enemies);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();

private:
	Vector2 AdjustedFieldSize = {};

	ThePlayer* Player = {};
	EnemyControl* Enemies = {};

};

