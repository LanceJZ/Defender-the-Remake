#include "Game.h"

Game::Game()
{
	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().
	PlayerID = Managers.EM.AddModel3D(Player = DBG_NEW ThePlayer());
	LogicID = Managers.EM.AddCommon(Logic = DBG_NEW GameLogic());
	EnemiesID = Managers.EM.AddCommon(Enemies = DBG_NEW EnemyControl());
	BackGroundID = Managers.EM.AddCommon(BackGround = DBG_NEW TheBackground());
}

Game::~Game()
{
}

bool Game::Initialize(Utilities* utilities) //Initialize
{
	Common::Initialize(utilities);

	Logic->SetPlayer(Player);
	Logic->SetEnemies(Enemies);

	Enemies->SetPlayer(Player);

	BackGround->SetPlayer(Player);

	float multi = 7.0f;
	FieldSize = { GetScreenWidth() * multi, (float)GetScreenHeight() };

	//Any Entities added after this point need this method fired manually.
	return true;
}

bool Game::Load()
{
	//Models
	for (size_t i = 0; i < 7; i++)
	{
		std::string name = "Ground";
		std::string nameR = "GroundRadar";
		name.append(std::to_string(i + 1));
		nameR.append(std::to_string(i + 1));

		Model land = Managers.CM.LoadAndGetModel(name);
		Model radarLand = Managers.CM.LoadAndGetModel(nameR);

		BackGround->SetLandPartsModel(land, radarLand, i);
	}

	Model cube = Managers.CM.LoadAndGetModel("Cube");

	BackGround->SetRadarModel(Managers.CM.LoadAndGetModel("RadarOutline"),
		Managers.CM.LoadAndGetModel("UIBottomSides"));
	BackGround->SetUIBackfaceModel(Managers.CM.LoadAndGetModel("UIBackface"));
	BackGround->SetStarModel(cube);

	Player->SetModel(Managers.CM.LoadAndGetModel("Player Ship"));
	Player->SetRadarModel(Managers.CM.LoadAndGetModel("Player Radar"));
	Player->SetFlameModel(Managers.CM.LoadAndGetModel("Player Flame"));
	Player->SetShotModels(Managers.CM.LoadAndGetModel("Player Shot"),
		Managers.CM.LoadAndGetModel("Player Shot Tail"));

	Logic->SetPersonModel(Managers.CM.LoadAndGetModel("Person"));
	Logic->SetPersonRadarModel(Managers.CM.LoadAndGetModel("Person Radar"));

	Enemies->SetLanderModel(Managers.CM.LoadAndGetModel("Lander"));
	Enemies->SetMutantModel(Managers.CM.LoadAndGetModel("Mutant"));
	Enemies->SetBomberModel(Managers.CM.LoadAndGetModel("Bomber"));
	Enemies->SetSwarmerModel(Managers.CM.LoadAndGetModel("Swarmer"));
	Enemies->SetPodModel(Managers.CM.LoadAndGetModel("Pod"));
	Enemies->SetShotModel(Managers.CM.LoadAndGetModel("Shot"));
	Enemies->SetBombModel(Managers.CM.LoadAndGetModel("Bomb"));
	Enemies->SetLanderRadarModel(Managers.CM.LoadAndGetModel("Lander Radar"));
	Enemies->SetMutantRadarModel(Managers.CM.LoadAndGetModel("Mutant Radar"));
	Enemies->SetBomberRadarModel(Managers.CM.LoadAndGetModel("Bomber Radar"));
	Enemies->SetSwarmerRadarModel(Managers.CM.LoadAndGetModel("Swarmer Radar"));
	Enemies->SetPodRadarModel(Managers.CM.LoadAndGetModel("Pod Radar"));
	Enemies->SetBaiterModel(Managers.CM.LoadAndGetModel("Baiter"));
	Enemies->SetBaiterRadarModel(Managers.CM.LoadAndGetModel("Baiter Radar"));

	Particles.SetCubeModel(cube);

	//Sounds
	Player->SetFireSound(Managers.CM.LoadAndGetSound("Player Fire"));
	Player->SetExplosionSound(Managers.CM.LoadAndGetSound("Player Explode"));
	Player->SetThrustSound(Managers.CM.LoadAndGetSound("Player Thrust"));
	Player->SetSmartbombSound(Managers.CM.LoadAndGetSound("Smartbomb"));
	Player->SetBonusSound(Managers.CM.LoadAndGetSound("Bonus"));

	Enemies->SetBaiterSpawnSound(Managers.CM.LoadAndGetSound("Baiter Spawn"));
	Enemies->SetBomberExplodeSound(Managers.CM.LoadAndGetSound("Bomber Explode"));
	Enemies->SetExplodeSound(Managers.CM.LoadAndGetSound("Enemy Explode"));
	Enemies->SetFireSound(Managers.CM.LoadAndGetSound("Enemy Fire"));
	Enemies->SetLanderMutateSound(Managers.CM.LoadAndGetSound("Lander Mutate"));
	Enemies->SetLandersSpawnSound(Managers.CM.LoadAndGetSound("Landers Spawn"));
	Enemies->SetMutantFireSound(Managers.CM.LoadAndGetSound("Mutant Fire"));
	Enemies->SetPodExplodeSound(Managers.CM.LoadAndGetSound("Pod Explode"));
	Enemies->SetSwarmerExplodeSound(Managers.CM.LoadAndGetSound("Swarmer Explode"));
	Enemies->SetSwarmerFireSound(Managers.CM.LoadAndGetSound("Swarmer Fire"));
	Enemies->SetPersonDroppedSound(Managers.CM.LoadAndGetSound("Person Dropped"));
	Enemies->SetPersonGrabbedSound(Managers.CM.LoadAndGetSound("Person Grabbed"));

	Logic->SetPersonCaughtSound(Managers.CM.LoadAndGetSound("Person Caught"));
	Logic->SetPersonDroppedSound(Managers.CM.LoadAndGetSound("Person Dropped"));
	Logic->SetPersonGrabbedSound(Managers.CM.LoadAndGetSound("Person Grabbed"));
	Logic->SetPersonLandedSound(Managers.CM.LoadAndGetSound("Person Landed"));
	Logic->SetPersonSplatSound(Managers.CM.LoadAndGetSound("Person Splat"));

	return true;
}

bool Game::BeginRun()
{
	//Any Entities added after this point need this method fired manually.

	return true;
}

void Game::ProcessInput()
{
	GameInput();
}


void Game::Update(float deltaTime)
{
	if (Paused)
		return;

	if (Enemies->TriggerLandChange)
	{
		Enemies->TriggerLandChange = false;

		if (Enemies->NoMorePeople) BackGround->AllThePersonManDead();
		else BackGround->AllThePersonManNotDead();
	}
}

void Game::Draw3D()
{
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

void Game::GameInput()
{
	if (IsKeyPressed(KEY_P))
	{
		Paused = !Paused;
	}

	//if (State == MainMenu)
	//{
	//	if (IsGamepadAvailable(0))
	//	{
	//		if (IsGamepadButtonPressed(0, 15))//Start button
	//		{
	//			NewGame();
	//		}
	//	}

	//	if (IsKeyPressed(KEY_N))
	//	{
	//		NewGame();
	//	}

	//	if (IsKeyPressed(KEY_D))
	//	{

	//	}
	//}

	//if (State == InPlay)
	//{
	//	if (IsGamepadAvailable(0))
	//	{
	//		if (IsGamepadButtonPressed(0, 13)) //Menu Button
	//		{
	//			State = Pause;
	//		}

	//		if (IsGamepadButtonPressed(0, 8)) //X button
	//		{
	//			PlayBackgroundMusic = !PlayBackgroundMusic;
	//		}
	//	}

	//	if (IsKeyPressed(KEY_M))
	//	{
	//		PlayBackgroundMusic = !PlayBackgroundMusic;
	//	}


	//	if (IsKeyPressed(KEY_P))
	//	{
	//		State = Pause;
	//	}

	//	if (IsKeyPressed(KEY_B))
	//	{
	//	}
	//}
	//else if (State == Pause)
	//{
	//	if (IsKeyPressed(KEY_P))
	//	{
	//		State = InPlay;
	//	}

	//	if (IsGamepadAvailable(0))
	//	{
	//		if (IsGamepadButtonPressed(0, 13)) //Menu Button
	//		{
	//			State = InPlay;
	//		}
	//	}
	//}
}