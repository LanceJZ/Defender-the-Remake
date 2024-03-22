#include "ThePerson.h"

ThePerson::ThePerson()
{
}

ThePerson::~ThePerson()
{
}

bool ThePerson::Initialize(Utilities* utilities)
{
	Model3D::Initialize(utilities);

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

}

void ThePerson::Draw()
{
	Model3D::Draw();

}

void ThePerson::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}