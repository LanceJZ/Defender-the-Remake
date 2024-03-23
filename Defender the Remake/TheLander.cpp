#include "TheLander.h"

TheLander::TheLander()
{
}

TheLander::~TheLander()
{
}

void TheLander::SetPeople(ThePerson* people[10])
{
	for (int i = 0; i < 10; i++)
	{
		People[i] = people[i];
	}
}

bool TheLander::Initialize(Utilities* utilities)
{
	Enemy::Initialize(utilities);

	return false;
}

bool TheLander::BeginRun()
{
	Enemy::BeginRun();

	return false;
}

void TheLander::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

}

void TheLander::Draw()
{
	Model3D::Draw();

}

void TheLander::Spawn(Vector3 position)
{
	Entity::Spawn(position);

}