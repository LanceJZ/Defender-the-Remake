#pragma once
#include "Globals.h"

class ThePerson : public Model3D
{
public:
	ThePerson();
	virtual ~ThePerson();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position);

private:


};
