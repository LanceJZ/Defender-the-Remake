#include "GameLogic.h"

GameLogic::GameLogic()
{
	for (int i = 0; i < 10; i++)
	{
		PeopleIDs[i] = TheManagers.EM.AddModel3D(People[i] = DBG_NEW ThePerson());
	}
}

GameLogic::~GameLogic()
{
}

void GameLogic::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void GameLogic::SetEnemies(EnemyControl* enemies)
{
	Enemies = enemies;
}

void GameLogic::SetPersonModel(Model model)
{
	PersonModel = model;
}

void GameLogic::SetPersonRadarModel(Model model)
{
	PersonRadarModel = model;
}

bool GameLogic::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	for (auto person : People)
	{
		person->Initialize(utilities);
	}

	return false;
}

bool GameLogic::BeginRun()
{
	Common::BeginRun();

	Enemies->SetPeople(People);

	SetupPersonMan();
	SpawnPersonMan(10);

	return true;
}

void GameLogic::Update()
{
	Common::Update();

	UpdatePersonMan();

	if (Player->BeenHit)
	{
		 PlayerHitReset();
	}

	if (Player->SmartBombFired)
	{
		Enemies->SmartBomb();
	}
}

void GameLogic::UpdatePersonMan()
{
}

void GameLogic::SpawnPersonMan(int count)
{
	for (int i = 0; i < count; i++)
	{
		float x = GetRandomFloat(-AdjustedFieldSize.x, AdjustedFieldSize.x);
		float y = GetScreenHeight() * 0.5f - (People[i]->VerticesSize * 2.0f);
		People[i]->Spawn({x, y, 0.0f});
	}
}

void GameLogic::SetupPersonMan()
{
	for (auto person : People)
	{
		person->SetPlayer(Player);
		person->SetModel(PersonModel);
		person->SetRadarModel(PersonRadarModel);
		person->BeginRun();
	}
}

void GameLogic::PlayerHitReset()
{
	Enemies->ResetField();
	Player->BeenHit = false;
}

void GameLogic::ResetField()
{
	Enemies->ResetField();
	TheManagers.PM.Reset();
}
