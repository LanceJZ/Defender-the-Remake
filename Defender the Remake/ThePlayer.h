#pragma once
#include "Globals.h"
#include "Common.h"
#include "Utilities.h"
#include "Shot.h"

class ThePlayer : public Model3D
{
public:
	ThePlayer();
	virtual ~ThePlayer();

	bool NewLife = false;
	bool GameOver = false;
	bool SmartBombFired = false;

	int Score { 0 };
	int HighScore { 0 };
	int Lives { 0 };

	void SetShotModels(Model shotModel, Model tailModel);
	void SetFlameModel(Model model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Input();
	void Update(float deltaTime);
	void Draw();

	void Hit();
	void ScoreUpdate(int addToScore);
	void Reset();
	void NewGame();

private:
	int NextNewLifeScore = 10000;
	bool FacingRight = true;
	bool ChangedFacing = false;
	bool RotateFacing = false;
	int SmartBombs = 0;
	float MoveToOffset = 1000;
	float HorzSpeed = 30.0f;
	float HorzMaxSpeed = 200.0f;
	float HorzDrag = 2.0f;
	float ForwardAcceleration = 40.0f;
	float MaxForwardV = 1000.0f;
	float ForwardDrag = 20.0f;
	float AirDrag = 0.004f;

	Model3D* Flame = {};
	Model ShotModel = {};
	Model ShotTailModel = {};

	void Thrust();
	void ThrustOff();
	void Reverse();
	void MoveLeft();
	void MoveRight();
	void RotateShipFacing();

	void CameraMovement(float deltaTime);

	void Gamepad();
	void Keyboard();
};
