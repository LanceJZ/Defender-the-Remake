#include "Common.h"

bool Common::Initialize()
{
	WindowWidth = GetScreenWidth();
	WindowHeight = GetScreenHeight();
	GameWindowHalfHeight = (int)(WindowHeight * 0.5f);
	GameWindowHalfWidth = (int)(WindowWidth * 0.5f);

	return false;
}

bool Common::BeginRun()
{
	return false;
}

void Common::Input()
{
}

void Common::Update()
{
}

void Common::FixedUpdate()
{
}

void Common::Draw2D()
{
}