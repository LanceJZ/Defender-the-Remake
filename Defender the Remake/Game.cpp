#include "Game.h"

Game::Game()
{
	//When adding classes to EM, must be pointer to heap,IE: Name = new Class().
	PlayerID = EM.AddModel3D(Player = DBG_NEW ThePlayer());
	LogicID = EM.AddCommon(Logic = DBG_NEW GameLogic());
	EnemiesID = EM.AddCommon(Enemies = DBG_NEW EnemyControl());
	BackGroundID = EM.AddCommon(BackGround = DBG_NEW TheBackground());
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

	Logic->SetBackground(BackGround);

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

		Model land = CM.LoadAndGetModel(name);
		Model radarLand = CM.LoadAndGetModel(nameR);

		BackGround->SetLandPartsModel(land, radarLand, i);
	}

	Model cube = CM.LoadAndGetModel("Cube");

	BackGround->SetRadarModel(CM.LoadAndGetModel("RadarOutline"),
		CM.LoadAndGetModel("UIBottomSides"));
	BackGround->SetUIBackfaceModel(CM.LoadAndGetModel("UIBackface"));
	BackGround->SetStarModel(cube);

	Player->SetModel(CM.LoadAndGetModel("Player Ship"));
	Player->SetRadarModel(CM.LoadAndGetModel("Player Radar"));
	Player->SetFlameModel(CM.LoadAndGetModel("Player Flame"));
	Player->SetShotModels(CM.LoadAndGetModel("Player Shot"),
		CM.LoadAndGetModel("Player Shot Tail"));

	Logic->SetPersonModel(CM.LoadAndGetModel("Person"));
	Logic->SetPersonRadarModel(CM.LoadAndGetModel("Person Radar"));
	Logic->SetSmartBombModel(CM.LoadAndGetModel("BombIcon"));

	Enemies->SetLanderModel(CM.LoadAndGetModel("Lander"));
	Enemies->SetMutantModel(CM.LoadAndGetModel("Mutant"));
	Enemies->SetBomberModel(CM.LoadAndGetModel("Bomber"));
	Enemies->SetSwarmerModel(CM.LoadAndGetModel("Swarmer"));
	Enemies->SetPodModel(CM.LoadAndGetModel("Pod"));
	Enemies->SetShotModel(CM.LoadAndGetModel("Shot"));
	Enemies->SetBombModel(CM.LoadAndGetModel("Bomb"));
	Enemies->SetLanderRadarModel(CM.LoadAndGetModel("Lander Radar"));
	Enemies->SetMutantRadarModel(CM.LoadAndGetModel("Mutant Radar"));
	Enemies->SetBomberRadarModel(CM.LoadAndGetModel("Bomber Radar"));
	Enemies->SetSwarmerRadarModel(CM.LoadAndGetModel("Swarmer Radar"));
	Enemies->SetPodRadarModel(CM.LoadAndGetModel("Pod Radar"));
	Enemies->SetBaiterModel(CM.LoadAndGetModel("Baiter"));
	Enemies->SetBaiterRadarModel(CM.LoadAndGetModel("Baiter Radar"));

	Particles.SetCubeModel(cube);

	//Sounds
	Player->SetFireSound(CM.LoadAndGetSound("Player Fire"));
	Player->SetExplosionSound(CM.LoadAndGetSound("Player Explode"));
	Player->SetThrustSound(CM.LoadAndGetSound("Player Thrust"));
	Player->SetSmartbombSound(CM.LoadAndGetSound("Smartbomb"));
	Player->SetBonusSound(CM.LoadAndGetSound("Bonus"));

	Enemies->SetBaiterSpawnSound(CM.LoadAndGetSound("Baiter Spawn"));
	Enemies->SetBomberExplodeSound(CM.LoadAndGetSound("Bomber Explode"));
	Enemies->SetExplodeSound(CM.LoadAndGetSound("Enemy Explode"));
	Enemies->SetFireSound(CM.LoadAndGetSound("Enemy Fire"));
	Enemies->SetLanderMutateSound(CM.LoadAndGetSound("Lander Mutate"));
	Enemies->SetLandersSpawnSound(CM.LoadAndGetSound("Landers Spawn"));
	Enemies->SetMutantFireSound(CM.LoadAndGetSound("Mutant Fire"));
	Enemies->SetPodExplodeSound(CM.LoadAndGetSound("Pod Explode"));
	Enemies->SetSwarmerExplodeSound(CM.LoadAndGetSound("Swarmer Explode"));
	Enemies->SetSwarmerFireSound(CM.LoadAndGetSound("Swarmer Fire"));
	Enemies->SetPersonDroppedSound(CM.LoadAndGetSound("Person Dropped"));
	Enemies->SetPersonGrabbedSound(CM.LoadAndGetSound("Person Grabbed"));

	Logic->SetPersonCaughtSound(CM.LoadAndGetSound("Person Caught"));
	Logic->SetPersonDroppedSound(CM.LoadAndGetSound("Person Dropped"));
	Logic->SetPersonGrabbedSound(CM.LoadAndGetSound("Person Grabbed"));
	Logic->SetPersonLandedSound(CM.LoadAndGetSound("Person Landed"));
	Logic->SetPersonSplatSound(CM.LoadAndGetSound("Person Splat"));

	return true;
}

bool Game::BeginRun()
{
	//Any Entities added after this point need this method fired manually.

	return true;
}

void Game::Input()
{
	if (IsKeyPressed(KEY_P))
	{
		if (Logic->State == Pause) Logic->State = InPlay;
		else if (Logic->State == InPlay) Logic->State = Pause;
	}

	if (Logic->State == GameOver)
	{
		if (IsKeyPressed(KEY_N)) Logic->NewGame();
	}
}

void Game::Update(float deltaTime)
{
	//for (const auto& lander : Enemies->Landers) lander->CheckShotCollisions();

	//for (const auto& mutant : Enemies->Mutants) mutant->CheckShotCollisions();

	//for (const auto& bomber : Enemies->Bombers) bomber->CheckShotCollisions();

	//for (const auto& swarmer : Enemies->Swarmers) swarmer->CheckShotCollisions();
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