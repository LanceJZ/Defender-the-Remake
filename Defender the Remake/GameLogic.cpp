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
	Enemies->SetPeople(People);

	return false;
}

bool GameLogic::BeginRun()
{

	return false;
}

void GameLogic::Update()
{
	Common::Update();

	UpdatePersonMan();
}

void GameLogic::UpdatePersonMan()
{
}
