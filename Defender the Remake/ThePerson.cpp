#include "ThePerson.h"

ThePerson::ThePerson()
{
	TheManagers.EM.AddModel3D(MirrorL = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(MirrorR = DBG_NEW Model3D());
	TheManagers.EM.AddModel3D(Radar = DBG_NEW Model3D());
}

ThePerson::~ThePerson()
{
}

void ThePerson::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void ThePerson::SetRadarModel(Model model)
{
	Radar->SetModel(model);
}

bool ThePerson::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

	Radar->Initialize(utilities);

	float mirrorMultiplier = GetScreenWidth() * 7.0f;

	MirrorL->SetModel(GetModel());
	MirrorR->SetModel(GetModel());

	MirrorL->X(X() - mirrorMultiplier);
	MirrorR->X(X() + mirrorMultiplier);

	MirrorL->SetParent(this);
	MirrorR->SetParent(this);

	float comp = 0.062f;
	float ww = 3.5f;
	float swww = (GetScreenWidth() * ww);

	WidthOffset = (swww * 2) * comp;
	WidthCameraOffset = comp;
	WidthMirrorOffset = swww * comp;
	HeightMultiplier = GetScreenHeight() * 0.4376f;

	return false;
}

bool ThePerson::BeginRun()
{
	Model3D::BeginRun();

	return false;
}

void ThePerson::Update(float deltaTime)
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

	Radar->Y((Y() * 0.158f) - HeightMultiplier);
}

void ThePerson::Draw()
{
	Model3D::Draw();

}

void ThePerson::Spawn(Vector3 position)
{
	Entity::Spawn(position);

	MirrorL->Enabled = true;
	MirrorR->Enabled = true;
	Radar->Enabled = true;
}

void ThePerson::Dropped()
{
}

void ThePerson::Destroy()
{
	Entity::Destroy();

	MirrorL->Enabled = false;
	MirrorR->Enabled = false;
	Radar->Enabled = false;
}

void ThePerson::Reset()
{
}
