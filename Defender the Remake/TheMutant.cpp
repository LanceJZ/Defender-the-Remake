#include "TheMutant.h"

TheMutant::TheMutant()
{
}

TheMutant::~TheMutant()
{
}

bool TheMutant::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	return false;
}

bool TheMutant::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheMutant::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

}

void TheMutant::Draw()
{
	Model3D::Draw();

}

void TheMutant::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}