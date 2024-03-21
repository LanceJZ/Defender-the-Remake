#pragma once
#include "Common.h"
#include "Globals.h"
#include "ThePlayer.h"
#include "Shot.h"

class Enemy : public Model3D
{
public:
	Enemy();
	virtual ~Enemy();

	bool CountChange = false;
	int ScoreAmount = 0;
	Shot* Shots[4] = { nullptr };

	void SetPlayer(ThePlayer* player);
	void SetRadarModel(Model model);
	void SetShotModel(Model model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Spawn(Vector3 position);

	void Update(float deltaTime);
	void Draw();

protected:
	size_t ShotTimerID = 0;

	ThePlayer* Player = nullptr;


private:


};
