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

bool GameLogic::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	return false;
}

bool GameLogic::BeginRun()
{
	Enemies->SetPeople(People);

	SetupPersonMan();

	return true;
}

void GameLogic::Update()
{
	Common::Update();

	UpdatePersonMan();
}

void GameLogic::UpdatePersonMan()
{
}

void GameLogic::SpawnPersonMan(int count)
{
	for (int i = 0; i < count; i++)
	{
		float x = GetRandomFloat(-AdjustedFieldSize.x, AdjustedFieldSize.x);
		float y = GetScreenHeight() * 0.49f;
		People[i]->Position = { x, y, 0.0f };
	}
}

void GameLogic::SetupPersonMan()
{
	for (auto person : People)
	{
		person->SetModel(PersonModel);
	}

	SpawnPersonMan(10);
}
