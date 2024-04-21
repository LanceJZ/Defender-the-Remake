#include "ThePerson.h"

ThePerson::ThePerson()
{
}

ThePerson::~ThePerson()
{
}

bool ThePerson::Initialize(Utilities* utilities)
{
	MirrorRadar::Initialize(utilities);

	return false;
}

bool ThePerson::BeginRun()
{
	MirrorRadar::BeginRun();

	return false;
}

void ThePerson::Update(float deltaTime)
{
	MirrorRadar::Update(deltaTime);

}

void ThePerson::Draw()
{
	MirrorRadar::Draw();
}

void ThePerson::Spawn(Vector3 position)
{
	MirrorRadar::Spawn(position);

}

void ThePerson::Dropped()
{

}

void ThePerson::Destroy()
{
	MirrorRadar::Destroy();

}

void ThePerson::Reset()
{

}
