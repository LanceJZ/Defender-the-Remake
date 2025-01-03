#include "ThrustControl.h"

ThrustControl::ThrustControl()
{

}

ThrustControl::~ThrustControl()
{
}

bool ThrustControl::Initialize()
{
	ParticleManager::Initialize();

	return false;
}

bool ThrustControl::BeginRun()
{
	ParticleManager::BeginRun();

	return false;
}

void ThrustControl::FixedUpdate()
{
	ParticleManager::FixedUpdate();

}

void ThrustControl::SpawnCubes(Vector3 position, Vector3 velocity, float endWidth,
	float length, int count, float time, Color color)
{
	float size = 3.1f;
	float speed = 5.1f;

	float width = M.GetRandomFloat(-endWidth, endWidth);

	velocity = Vector3Add(M.GetVelocityFromAngleZ(width, length), velocity);

	ParticleManager::SpawnCubes(position, velocity, size, speed, count, time, color);
}
