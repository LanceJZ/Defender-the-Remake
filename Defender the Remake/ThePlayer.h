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
	bool EnemyHit = false;

	int Score { 0 };
	int HighScore { 0 };
	int Lives { 0 };

	Shot* Shots[4] = { nullptr };

	void SetShotModels(Model shotModel, Model tailModel);
	void SetFlameModel(Model model);
	void SetRadarModel(Model model);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Input();
	void Update(float deltaTime);
	void Draw();

	bool GetCollusion(Entity& entity);
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
	float CameraFacingOffset = 0.0f;
	float MoveToOffset = 1000;
	float HorzSpeed = 30.0f;
	float HorzMaxSpeed = 200.0f;
	float HorzDrag = 2.0f;
	float ForwardAcceleration = 40.0f;
	float MaxForwardV = 1000.0f;
	float ForwardDrag = 20.0f;
	float AirDrag = 0.004f;
	float RadarModifier = 0.0f;

	Entity* CollusionBack = nullptr;
	Entity* CollusionMidFront = nullptr;
	Entity* CollusionFront = nullptr;
	Entity* CollusionTip = nullptr;
	Model3D* Flame = {};
	Model3D* Radar = {};
	Model ShotModel = {};
	Model ShotTailModel = {};

	void Thrust();
	void ThrustOff();
	void Reverse();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void HorizontalFriction();
	void RotateShipFacing();
	void Fire();

	void CameraMovement(float deltaTime);
	void RadarMovement(float deltaTime);

	void Gamepad();
	void Keyboard();
};
