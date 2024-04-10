#pragma once
#include "Enemy.h"
#include "ThePerson.h"

enum StateList
{
	LoweringToSeek,
	Seek,
	FoundPersonMan,
	TakePersonMan,
	Mutate
};

class TheLander : public Enemy
{
public:
	TheLander();
	virtual ~TheLander();

	bool MutateLander = false;

	void SetPeople(ThePerson* people[10]);
	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update(float deltaTime);
	void Draw();

	void Spawn(Vector3 position);
	void Reset();
	void Hit();

private:
	size_t PersonTargetID = 0;
	size_t SeekTimerID = 0;
	int SeekTimerChance = 0;
	float GroundFloorY = 0;
	float GroundCeilingY = 0;
	float SeekTimerAmountMax = 0;
	float SeekTimerAmountMin = 0;
	StateList State = LoweringToSeek;
	Sound MutateSound = { 0 };

	ThePerson* People[10] = {};

	void GoToSeek();
	void SeekPersonMan();
	void GoingDown();
	void GrabPersonMan();
	void SpawnMutant();
	bool CheckCollision();
	void Destroy();
};
