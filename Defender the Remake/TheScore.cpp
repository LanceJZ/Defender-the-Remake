#include "TheScore.h"

TheScore::TheScore()
{

}

TheScore::~TheScore()
{
}

bool TheScore::Initialize(Utilities* utilities)
{
	OnScreenText::Initialize(TheUtilities);

	return false;
}

bool TheScore::BeginRun()
{

	return false;
}

void TheScore::Update()
{
	OnScreenText::Update();

}

void TheScore::Draw2D()
{
	OnScreenText::Draw2D();

	DrawText(std::to_string(Score).c_str(),
		100 - (1 * 45), 75, 45, BLUE);
}

void TheScore::AddToScore(int amount)
{
	Score += amount;

	if (Score > HighScore)
	{
		HighScore = Score;
	}
}

void TheScore::ClearScore()
{
	Score = 0;
}

unsigned int TheScore::GetScore()
{
	return Score;
}