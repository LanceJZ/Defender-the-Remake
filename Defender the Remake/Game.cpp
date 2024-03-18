#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Initialize(Utilities &utilities, GameLogic* gameLogic) //Initialize
{
	TheUtilities = &utilities;
	Logic = gameLogic;

	Common::Initialize(&utilities);

	float multi = 4.0f;
	FieldSize = { GetScreenWidth() * multi, (float)GetScreenHeight() };

	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().
	LogicID = TheManagers.EM.AddCommon(Logic = DBG_NEW GameLogic());
	BackGroundID = TheManagers.EM.AddCommon(BackGround = DBG_NEW TheBackground());
	EnemiesID = TheManagers.EM.AddCommon(Enemies = DBG_NEW EnemyControl());
	PlayerID = TheManagers.EM.AddModel3D(Player = DBG_NEW ThePlayer());

	Logic->SetPlayer(Player);
	Logic->SetEnemies(Enemies);

	Enemies->SetPlayer(Player);

	//Any Entities added after this point need this method fired manually.
	TheManagers.Initialize();
	return true;
}

bool Game::Load()
{
	Player->SetModel(TheManagers.CM.LoadAndGetModel("Player Ship"));

	for (size_t i = 0; i < 7; i++)
	{
		std::string name = "Ground";
		std::string nameR = "GroundRadar";
		name.append(std::to_string(i + 1));
		nameR.append(std::to_string(i + 1));

		Model land = TheManagers.CM.LoadAndGetModel(name);
		Model radarLand = TheManagers.CM.LoadAndGetModel(nameR);

		BackGround->SetLandParts(land, radarLand, i);
	}



	return true;
}

bool Game::BeginRun()
{
	//Any Entities added after this point need this method fired manually.
	TheManagers.BeginRun();


	NewGame();

	return true;
}

void Game::ProcessInput()
{
	GameInput();
	TheManagers.EM.Input();
}


void Game::Update(float deltaTime)
{
	if (State == Pause)
		return;

	TheManagers.EM.Update(deltaTime);
}

void Game::Draw()
{
	BeginMode3D(TheCamera);

	//3D Drawing here.
	Draw3D();

	EndMode3D();

	//2D drawing, fonts go here.

	Draw2D();
}

void Game::Draw3D()
{
	TheManagers.EM.Draw3D();

#ifdef _DEBUG
	int fsx = int(FieldSize.x * 0.5f);
	int fsy = int(FieldSize.y * 0.5f);

	DrawLine(-fsx, -fsy, fsx, -fsy, { DARKBLUE });
	DrawLine(fsx, -fsy, fsx, fsy, { DARKBLUE });
	DrawLine(fsx, fsy - 1, -fsx, fsy, { DARKBLUE });
	DrawLine(-fsx, fsy - 1, -fsx, -fsy, { DARKBLUE });
#endif
}

void Game::Draw2D()
{
}

void Game::NewGame()
{
	State = InPlay;

}

void Game::GameInput()
{
	if (State == MainMenu)
	{
		if (IsGamepadAvailable(0))
		{
			if (IsGamepadButtonPressed(0, 15))//Start button
			{
				NewGame();
			}
		}

		if (IsKeyPressed(KEY_N))
		{
			NewGame();
		}

		if (IsKeyPressed(KEY_D))
		{

		}
	}

	if (State == InPlay)
	{
		if (IsGamepadAvailable(0))
		{
			if (IsGamepadButtonPressed(0, 13)) //Menu Button
			{
				State = Pause;
			}

			if (IsGamepadButtonPressed(0, 8)) //X button
			{
				PlayBackgroundMusic = !PlayBackgroundMusic;
			}
		}

		if (IsKeyPressed(KEY_M))
		{
			PlayBackgroundMusic = !PlayBackgroundMusic;
		}


		if (IsKeyPressed(KEY_P))
		{
			State = Pause;
		}

		if (IsKeyPressed(KEY_B))
		{
		}
	}
	else if (State == Pause)
	{
		if (IsKeyPressed(KEY_P))
		{
			State = InPlay;
		}

		if (IsGamepadAvailable(0))
		{
			if (IsGamepadButtonPressed(0, 13)) //Menu Button
			{
				State = InPlay;
			}
		}
	}
}