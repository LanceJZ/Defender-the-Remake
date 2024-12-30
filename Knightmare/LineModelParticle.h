#pragma once
#include "LineModel.h"
#include "EntityManager.h"
#include "KnightMath.h"

class LineModelParticle : public LineModel
{
public:
	LineModelParticle();
	virtual ~LineModelParticle();

	void SetEntityManager(EntityManager *entityManager);

	bool Initialize();
	bool BeginRun();

	void Update(float deltaTime);
	void Draw3D();

	void Spawn(Vector3 position, Vector3 velocity,
		float rotationZ, float maxSpeed,float maxTime);
	void Destroy();

private:
	size_t LifeTimerID = 0;

	EntityManager* EM = nullptr;
	KnightMath M = {};
};
