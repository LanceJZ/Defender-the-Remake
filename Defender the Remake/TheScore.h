#pragma once
#include "Globals.h"
#include "OnScreenText.h"

class TheScore : public OnScreenText
{
public:
	TheScore();
	virtual ~TheScore();

	bool Initialize(Utilities* utilities);
	bool BeginRun();

	void Update();
	void Draw2D();

	void AddToScore(int amount);
	void ClearScore();

	unsigned int GetScore();

private:
	unsigned int Score = 0;
	unsigned int HighScore = 21270;
};

