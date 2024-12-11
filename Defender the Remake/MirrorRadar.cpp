#include "MirrorRadar.h"

MirrorRadar::MirrorRadar()
{
	EM.AddModel3D(MirrorL = DBG_NEW Model3D());
	EM.AddModel3D(MirrorR = DBG_NEW Model3D());
	EM.AddModel3D(Radar = DBG_NEW Model3D());
}

MirrorRadar::~MirrorRadar()
{
}

void MirrorRadar::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void MirrorRadar::SetRadarModel(Model model)
{
	Radar->SetModel(model);
}

bool MirrorRadar::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	return true;
}

bool MirrorRadar::BeginRun()
{
	Model3D::BeginRun();

	float mirrorMultiplier = GetScreenWidth() * 7.0f;

	Radar->HideCollision = true;

	MirrorL->SetModel(Get3DModel());
	MirrorR->SetModel(Get3DModel());
	MirrorL->HideCollision = true;
	MirrorR->HideCollision = true;

	MirrorL->X(-mirrorMultiplier);
	MirrorR->X(mirrorMultiplier);

	MirrorL->SetParent(*this);
	MirrorR->SetParent(*this);

	float comp = 0.064f;
	float ww = 3.5f;
	float swww = (GetScreenWidth() * ww);

	WidthOffset = (swww * 2) * comp;
	WidthCameraOffset = comp;
	WidthMirrorOffset = swww * comp;
	HeightMultiplier = GetScreenHeight() * 0.4376f;

	return true;
}

void MirrorRadar::Update(float deltaTime)
{
	Model3D::Update(deltaTime);
}

void MirrorRadar::AlwaysUpdate(float deltaTime)
{
	Model3D::AlwaysUpdate(deltaTime);
}

void MirrorRadar::FixedUpdate(float deltaTime)
{
	Model3D::FixedUpdate(deltaTime);

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

	Radar->Y((Y() * 0.158f) - HeightMultiplier);
}

void MirrorRadar::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;
	Radar->Enabled = true;
}

void MirrorRadar::Destroy()
{
	Entity::Destroy();

	MirrorL->Enabled = false;
	MirrorR->Enabled = false;
	Radar->Enabled = false;
}