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
	//Logic = gameLogic; //This is not bing used yet.

	Common::Initialize(&utilities);

	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().
	PlayerID = TheManagers.EM.AddModel3D(Player = DBG_NEW ThePlayer());
	LogicID = TheManagers.EM.AddCommon(Logic = DBG_NEW GameLogic());
	EnemiesID = TheManagers.EM.AddCommon(Enemies = DBG_NEW EnemyControl());
	BackGroundID = TheManagers.EM.AddCommon(BackGround = DBG_NEW TheBackground());

	Logic->SetPlayer(Player);
	Logic->SetEnemies(Enemies);

	Enemies->SetPlayer(Player);

	float multi = 7.0f;
	FieldSize = { GetScreenWidth() * multi, (float)GetScreenHeight() };

	//Any Entities added after this point need this method fired manually.
	TheManagers.Initialize();
	return true;
}

bool Game::Load()
{
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

	BackGround->SetPlayer(Player);
	BackGround->SetRadar(TheManagers.CM.LoadAndGetModel("RadarOutline"),
		TheManagers.CM.LoadAndGetModel("UIBottomSides"));
	BackGround->SetUIBackface(TheManagers.CM.LoadAndGetModel("UIBackface"));

	Player->SetModel(TheManagers.CM.LoadAndGetModel("Player Ship"));
	Player->SetRadarModel(TheManagers.CM.LoadAndGetModel("Player Radar"));
	Player->SetFlameModel(TheManagers.CM.LoadAndGetModel("Player Flame"));
	Player->SetShotModels(TheManagers.CM.LoadAndGetModel("Player Shot"),
		TheManagers.CM.LoadAndGetModel("Player Shot Tail"));

	Logic->SetPersonModel(TheManagers.CM.LoadAndGetModel("Person"));
	Logic->SetPersonRadarModel(TheManagers.CM.LoadAndGetModel("Person Radar"));

	Enemies->SetLanderModel(TheManagers.CM.LoadAndGetModel("Lander"));
	Enemies->SetMutantModel(TheManagers.CM.LoadAndGetModel("Mutant"));
	Enemies->SetBomberModel(TheManagers.CM.LoadAndGetModel("Bomber"));
	Enemies->SetSwarmerModel(TheManagers.CM.LoadAndGetModel("Swarmer"));
	Enemies->SetPodModel(TheManagers.CM.LoadAndGetModel("Pod"));
	Enemies->SetShotModel(TheManagers.CM.LoadAndGetModel("Shot"));
	Enemies->SetBombModel(TheManagers.CM.LoadAndGetModel("Bomb"));
	Enemies->SetLanderRadarModel(TheManagers.CM.LoadAndGetModel("Lander Radar"));
	Enemies->SetMutantRadarModel(TheManagers.CM.LoadAndGetModel("Mutant Radar"));
	Enemies->SetBomberRadarModel(TheManagers.CM.LoadAndGetModel("Bomber Radar"));
	Enemies->SetSwarmerRadarModel(TheManagers.CM.LoadAndGetModel("Swarmer Radar"));
	Enemies->SetPodRadarModel(TheManagers.CM.LoadAndGetModel("Pod Radar"));

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