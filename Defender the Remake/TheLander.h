#pragma once
#include "Globals.h"
#include "Enemy.h"
#include "ThePerson.h"

class TheLander : public Enemy
{
public:
	TheLander();
	virtual ~TheLander();

	void SetPeople(ThePerson* people[10]);
	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Spawn(Vector3 position);

	void Update(float deltaTime);
	void Draw();

private:

	ThePerson* People[10] = {};

};
