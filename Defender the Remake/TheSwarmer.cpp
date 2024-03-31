#include "TheSwarmer.h"

TheSwarmer::TheSwarmer()
{
}

TheSwarmer::~TheSwarmer()
{
}

bool TheSwarmer::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	return false;
}

bool TheSwarmer::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheSwarmer::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

}

void TheSwarmer::Draw()
{
	Enemy::Draw();

}

void TheSwarmer::Spawn(Vector3 position)
{
	Enemy::Spawn(position);

}