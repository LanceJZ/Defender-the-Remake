#include "GameLogic.h"

GameLogic::GameLogic()
{
	for (int i = 0; i < 10; i++)
	{
		PeopleIDs[i] = Managers.EM.AddModel3D(People[i] = DBG_NEW ThePerson());
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

void GameLogic::SetPersonGrabbedSound(Sound sound)
{
	PersonGrabbedSound = sound;
}

void GameLogic::SetPersonDroppedSound(Sound sound)
{
	PersonDroppedSound = sound;
}

void GameLogic::SetPersonCaughtSound(Sound sound)
{
	PersonCaughtSound = sound;
}

void GameLogic::SetPersonLandedSound(Sound sound)
{
	PersonLandedSound = sound;
}

void GameLogic::SetPersonSplatSound(Sound sound)
{
	PersonSplatSound = sound;
}

bool GameLogic::Initialize(Utilities* utilities)
{
	Common::Initialize(utilities);

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	for (const auto& person : People)
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
	Wave = 1;

	return true;
}

void GameLogic::Update()
{
	Common::Update();

	if (Enemies->WaveEnded)
	{
		EndOfWave();
		return;
	}

	if (Player->BeenHit)
	{
		if (!Particles.GetParticlesActive()) PlayerHitReset();
	}

	if (Player->SmartBombFired)
	{
		Enemies->SmartBomb();
	}

	UpdatePersonMan();
}

void GameLogic::EndOfWave()
{
	Enemies->WaveEnded = false;
	NumberOfPeopleAlive = 0;

	for (const auto& person : People)
	{
		if (person->Enabled)
		{
			NumberOfPeopleAlive++;
		}
	}

	Player->ScoreUpdate(NumberOfPeopleAlive * (100 * Wave));
}

void GameLogic::UpdatePersonMan()
{
	Enemies->NoMorePeople = true;

	for (const auto& person : People)
	{
		if (person->Enabled)
		{
			Enemies->NoMorePeople = false;
			break;
		}
	}
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
	for (const auto& person : People)
	{
		person->SetPlayer(Player);
		person->SetModel(PersonModel);
		person->SetRadarModel(PersonRadarModel);
		person->SetCaughtSound(PersonCaughtSound);
		person->SetLandedSound(PersonLandedSound);
		person->SetSplatSound(PersonSplatSound);
		person->SetDroppedSound(PersonDroppedSound);
		person->BeginRun();
	}
}

void GameLogic::PlayerHitReset()
{
	Enemies->ResetField();
	Player->Reset();
}

void GameLogic::ResetField()
{
	Enemies->ResetField();
}
