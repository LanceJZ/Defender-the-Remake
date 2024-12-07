#pragma once
#include "Globals.h"
#include "Common.h"
#include "Utilities.h"
#include "Shot.h"
#include "TheScore.h"

class ThePlayer : public Model3D
{
public:
	ThePlayer();
	virtual ~ThePlayer();

	bool NewLife = false;
	bool GameOver = false;
	bool SmartBombFired = false;
	bool EnemyUpdate = false;
	bool SideWarped = false;
	bool Spawned = false;

	int SmartBombs = 0;
	int Lives = 0;

	Shot* Shots[4] = { nullptr };

	TheScore* Score;

	void SetShotModels(Model shotModel, Model tailModel);
	void SetFlameModel(Model model);
	void SetRadarModel(Model model);

	void SetFireSound(Sound sound);
	void SetExplosionSound(Sound sound);
	void SetThrustSound(Sound sound);
	void SetSmartbombSound(Sound sound);
	void SetBonusSound(Sound sound);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Input();
	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);

	bool GetCollusion(Entity& target);
	void Hit();
	void ScoreUpdate(int addToScore);
	void Reset();
	void Disable();
	void NewGame();

private:
	bool FacingRight = true;
	bool ChangedFacing = false;
	bool RotateFacing = false;
	unsigned int NextNewLifeScore = 10000;
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

	Sound FireSound = {};
	Sound ExplodeSound = {};
	Sound ThrustSound = {};
	Sound SmartbombSound = {};
	Sound BonusSound = {};

	Model ShotModel = {};
	Model ShotTailModel = {};

	Entity* CollusionBack = nullptr;
	Entity* CollusionMidFront = nullptr;
	Entity* CollusionFront = nullptr;
	Entity* CollusionTip = nullptr;
	Model3D* Flame = {};
	Model3D* Radar = {};

	void Thrust();
	void ThrustOff();
	void Reverse();
	void RotateShipFacing();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void HorizontalFriction();
	void Fire();
	void SmartBomb();

	void CameraMovement(float deltaTime);
	void RadarMovement(float deltaTime);

	void Gamepad();
	void Keyboard();
};
