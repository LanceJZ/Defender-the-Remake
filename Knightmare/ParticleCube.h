#pragma once
#include "Model3D.h"
#include "TheManagers.h"

class ParticleCube : public Model3D
{
public:
	ParticleCube();
	virtual ~ParticleCube();
	void SetManagers(TheManagers* Managers);

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(double deltaTime);
	void FixedUpdate(double deltaTime);
	void Draw3D();

	void Spawn(Vector3 position, Vector3 velocity, float radius, float speed, float time);

private:
	size_t LifeTimerID;

	TheManagers* Managers;
};
