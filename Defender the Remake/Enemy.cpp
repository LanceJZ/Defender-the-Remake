#include "Enemy.h"

Enemy::Enemy()
{
	for (size_t i = 0; i < 8; i++)
	{
		TheManagers.EM.AddModel3D(Shots[i] = DBG_NEW Shot());
	}

	ShotTimerID = TheManagers.EM.AddTimer();

	TheManagers.EM.AddModel3D(MirrorL = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(MirrorR = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(Radar = DBG_NEW Model3D());
}

Enemy::~Enemy()
{
}

void Enemy::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void Enemy::SetRadarModel(Model model)
{
	Radar->SetModel(model);
}

void Enemy::SetShotModel(Model model)
{
	for(auto shot : Shots)
	{
		shot->SetModel(model);
	}
}

bool Enemy::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	for(auto shot : Shots)
	{
		shot->Initialize(utilities);
	}

	Radar->Initialize(utilities);

	Xmultiplier = GetScreenWidth() * 2.75f;
	MirrorMultiplier = GetScreenWidth() * 7.0f;

	MirrorL->SetModel(GetModel());
	MirrorR->SetModel(GetModel());

	MirrorL->X(X() - MirrorMultiplier);
	MirrorR->X(X() + MirrorMultiplier);

	MirrorL->SetParent(this);
	MirrorR->SetParent(this);

	float comp = 0.062f;
	float ww = 3.5f;
	float swww = (GetScreenWidth() * ww);
	float swcalc = swww * comp;
	float swwwcalc = (swww * 2) * comp;

	WidthOffset = swwwcalc;
	WidthCameraOffset = comp;
	WidthMirrorOffset = swcalc;
	HeightMultiplier = GetScreenHeight() * 0.4376f;

	return true;
}

bool Enemy::BeginRun()
{
	Model3D::BeginRun();

	return false;
}

void Enemy::Update(float deltaTime)
{
	Model3D::Update(deltaTime);

	Radar->X(TheCamera.position.x + (-Player->X() * WidthCameraOffset) +
		(X() * WidthCameraOffset));

	if (Radar->X() > TheCamera.position.x + WidthMirrorOffset)
	{
		Radar->Position.x -= WidthOffset;
	}
	else if (Radar->X() < TheCamera.position.x - WidthMirrorOffset)
	{
		Radar->Position.x += WidthOffset;
	}

	Radar->Y((Y() * 0.148f) - HeightMultiplier);

	CheckPlayfieldSidesWarp(4.0f, 3.0f);
	CheckCollision();

}

void Enemy::Draw()
{
	Model3D::Draw();

}

void Enemy::FireShot()
{
	for (auto shot : Shots)
	{
		if (!shot->Enabled)
		{
			shot->EnemySpawn(Position,
				GetVelocityFromAngleZ(GetShotAngle(Position), 125.0f), 8.0f);
			return;
		}
	}
}

bool Enemy::CheckCollision()
{
	return false;
}

float Enemy::GetShotAngle(Vector3 position)
{
	float angle = 0;

	if (GetRandomValue(0, 100) > 50)
	{
		angle = AimedShot(position);
	}
	else
	{
		angle = GetRandomRadian();
	}

	return angle;
}
//TODO: add aiming for other side when player near edge.
float Enemy::AimedShot(Vector3 position)
{
	float percentChance = GetRandomFloat(0.0f, 0.05f);

	Vector3 aimXVelocity = Player->Position;
	aimXVelocity.x += Player->Velocity.x;

	return AngleFromVectorZ(aimXVelocity) +
		GetRandomFloat(-percentChance, percentChance);
}

void Enemy::Hit()
{
}

void Enemy::Reset()
{
}

void Enemy::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;
	Radar->Enabled = true;
}

void Enemy::Destroy()
{
	MirrorL->Enabled = false;
	MirrorR->Enabled = false;
	Radar->Enabled = false;
}