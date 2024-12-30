#include "GameLogic.h"

GameLogic::GameLogic()
{
	for (int i = 0; i < 10; i++)
	{
		PeopleIDs[i] = EM.AddModel3D(People[i] = DBG_NEW ThePerson());
	}

	NewWaveTimerID = EM.AddTimer(4.75f);
	PlayerDeathTimerID = EM.AddTimer();
	PlayerResetTimerID = EM.AddTimer(5.5f);
	WaveStartTimerID = EM.AddTimer(2.0f);

}

GameLogic::~GameLogic()
{
}

void GameLogic::SetBackground(TheBackground* background)
{
	BackGround = background;
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

void GameLogic::SetSmartBombModel(Model model)
{
	SmartBombModel = model;
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

bool GameLogic::Initialize()
{
	Common::Initialize();

	AdjustedFieldSize = Vector2Multiply(FieldSize, { 0.5f, 0.5f });

	for (const auto& person : People)
	{
		person->Initialize();
	}

	return true;
}

bool GameLogic::BeginRun()
{
	Common::BeginRun();

	SetupPersonMan();
	Enemies->SetPeople(People);
	State = GameOver;
	Player->GameOver = true;
	Enemies->GameEnded = true;

	return true;
}

void GameLogic::Input()
{
}

void GameLogic::Update()
{
	Common::Update();

	if (State == InPlay)
	{
		GameInPlay();
		LivesDisplayUpdate();
		SmartbombsDisplayUpdate();
	}

	if (State == NewWave)
	{
		NewWaveUpdate();
	}

	if (State == WaveStart)
	{
		WaveStarting();
	}

	if (State == PlayerHitByEnemy)
	{
		if (EM.TimerElapsed(PlayerResetTimerID)) PlayerHitReset();
	}

	if (Enemies->TriggerLandChange)
	{
		Enemies->TriggerLandChange = false;

		if (Enemies->NoMorePeople && !BackGround->WorldGone) BackGround->WorldExplode();
	}

	if (Player->KillPersonMan)
	{
		for (const auto& person : People)
		{
			person->Destroy();
		}

		Player->KillPersonMan = false;
		Player->EnemyUpdate = true;
		UpdatePersonMan();
	}
}

void GameLogic::Draw2D()
{
	if (State == NewWave)
	{
		std::string wave = "Attack Wave " + std::to_string(Wave);
		std::string bonus = "Bonus X " + std::to_string(NumberOfPersonManAlive * 150);

		DrawText(wave.c_str(),
			(int)(GameWindowHalfWidth - (40 * 13) * 0.25f),
			(int)(GameWindowHalfHeight - (40 * 3.5f)), 40, GRAY);

		DrawText("Completed", (int)(GameWindowHalfWidth - (40 * 9) * 0.25f),
			(int)(GameWindowHalfHeight - (40 * 2.5f)), 40, GRAY);

		DrawText(bonus.c_str(),
			(int)(GameWindowHalfWidth - (40 * 12) * 0.25f),
			(int)(GameWindowHalfHeight + (40 * 1.5f)), 40, GRAY);
	}

	if (State == WaveStart)
	{
		DrawText("Get Ready", (int)(GameWindowHalfWidth - (40 * 10) * 0.25f),
			(int)(GameWindowHalfHeight - (40 * 0.5f)), 40, GRAY);
	}

	if (State == GameOver)
	{
		DrawText("Game Over", (int)(GameWindowHalfWidth - (40 * 10) * 0.25f),
			(int)(GameWindowHalfHeight - (40 * 0.5f)), 40, GRAY);

		DrawText("Press N to start new game.",
			(int)(GameWindowHalfWidth - (40 * 26) * 0.25f),
			(int)(GameWindowHalfHeight + (40 * 1.5f)), 40, GRAY);
	}
}

void GameLogic::EndOfWave()
{
	Player->Disable();
	Enemies->WaveEnded = false;
	NumberOfPersonManAlive = 0;
	float x = TheCamera.position.x -90;

	for (size_t i = 0; i < 10; i++)
	{
		PersonManPosition[i] = People[i]->Position;
		People[i]->TurnOffRadar();
	}

	for (const auto& person : People)
	{
		if (person->Enabled)
		{
			NumberOfPersonManAlive++;
			person->Position.x = x;
			person->Position.y = GameWindowHalfHeight * 0.25f;

			x += person->Radius * 4.5f;
		}
	}

	Player->ScoreUpdate(NumberOfPersonManAlive * (100 * Wave));
	BackGround->NewWave();

	State = NewWave;
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
		float x = M.GetRandomFloat(-AdjustedFieldSize.x, AdjustedFieldSize.x);
		float y = GameWindowHalfHeight - (People[i]->VerticesSize * 2.0f);
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
	Player->Reset();
	BackGround->PlaceAllTheStars();
	Enemies->PlayerHitReset();
	Enemies->RestartWave();
	ResetField();
	LivesDisplay();
	State = InPlay;
	Enemies->RestartWaveTriggered = false;
	SmartbombsDisplay();
}

void GameLogic::GameInPlay()
{
	if (Enemies->WaveEnded)
	{
		EndOfWave();
		BackGround->StillTheStars();
		EM.ResetTimer(NewWaveTimerID);
		return;
	}

	if (Player->BeenHit)
	{
		Player->BeenHit = false;
		BackGround->StillTheStars();
		EM.ResetTimer(PlayerResetTimerID);
		State = PlayerHitByEnemy;
		Enemies->RestartWaveTriggered = true;

		if (Player->GameOver)
		{
			State = GameOver;
		}

		return;
	}
	else BackGround->ParallaxTheStars();

	if (Player->SmartBombFired)
	{
		Enemies->SmartBomb();
		SmartbombsDisplay();
	}

	UpdatePersonMan();
}

void GameLogic::NewWaveUpdate()
{
	if (EM.TimerElapsed(NewWaveTimerID))
	{
		EM.ResetTimer(WaveStartTimerID);
		BackGround->NewWaveDisplayDone();
		State = WaveStart;
		Player->Reset();
		Player->Update(GetFrameTime());
		Player->FixedUpdate(GetFrameTime());
		Player->Disable();
		BackGround->Update();
		BackGround->PlaceAllTheStars();

		for (size_t i = 0; i < 10; i++)
		{
			People[i]->TurnOnRadar();
			People[i]->Position = PersonManPosition[i];
		}
	}
}

void GameLogic::WaveStarting()
{
	BackGround->Update();

	if (EM.TimerElapsed(WaveStartTimerID))
	{
		State = InPlay;
		Player->Reset();
		Wave = Enemies->StartNewWave() + 1;
	}
}

void GameLogic::ResetField()
{
	for (const auto& person : People)
	{
		person->Reset();
	}
}

void GameLogic::LivesDisplay()
{
	size_t ships = PlayerLives.size();
	float row = -GameWindowHalfHeight + Player->Radius * 18.0f;

	if (Player->Lives > ships)
	{
		for (size_t i = 0; i < Player->Lives - ships; i++)
		{
			PlayerLives.push_back(DBG_NEW Model3D());
			EM.AddModel3D(PlayerLives.back(), Player->Get3DModel(),
				Player->ModelScale * 0.5f);
			PlayerLives.back()->Initialize();
			PlayerLives.back()->HideCollision = true;
			PlayerLives.back()->Position = { 0.0f , row, -200.0f };
			PlayerLives.back()->BeginRun();
		}
	}

	for (const auto& ship : PlayerLives)
	{
		ship->Enabled = false;
	}

	for (size_t i = 0; i < Player->Lives; i++)
	{
		PlayerLives[i]->Enabled = true;
	}
}

void GameLogic::LivesDisplayUpdate()
{
	float column = GameWindowHalfWidth - Player->Radius * 4.5f;

	for (const auto& ship : PlayerLives)
	{
		ship->X(-column + TheCamera.position.x);
		column -= Player->Radius * 6.0f;
	}
}

void GameLogic::SmartbombsDisplay()
{
	int bombs = (int)SmartbombIcons.size();
	float row = -GameWindowHalfWidth + 285.0f;

	if (Player->SmartBombs > bombs)
	{
		for (int i = 0; i < Player->SmartBombs - bombs; i++)
		{
			SmartbombIcons.push_back(DBG_NEW Model3D());
			EM.AddModel3D(SmartbombIcons.back(), SmartBombModel);
			SmartbombIcons.back()->Initialize();
			SmartbombIcons.back()->HideCollision = true;
			SmartbombIcons.back()->Position = { 0.0f , row, -200.0f };
			SmartbombIcons.back()->BeginRun();

			row -= 17.5f;
		}
	}

	for (auto& bomb : SmartbombIcons)
	{
		bomb->Enabled = false;
	}

	for (int i = 0; i < Player->SmartBombs; i++)
	{
		SmartbombIcons[i]->Enabled = true;
	}
}

void GameLogic::SmartbombsDisplayUpdate()
{
	float column = GameWindowHalfWidth - 341.25f;

	for (auto& bomb : SmartbombIcons)
	{
		bomb->X(-column + TheCamera.position.x);
	}
}

void GameLogic::NewGame()
{
	SpawnPersonMan(10);
	Wave = 1;
	State = InPlay;

	Player->NewGame();

	LivesDisplay();
	SmartbombsDisplay();

	Enemies->NewGame();
}
