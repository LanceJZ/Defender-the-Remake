#pragma warning ( push )
#pragma warning ( disable : 26495 )

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "raylib.h"
#include "Game.h"
#include "glfw/glfw3.h"

#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
	// allocations to be of _CLIENT_BLOCK type
#else
	#define DBG_NEW new
#endif

ContentManager CM = {};
EntityManager EM = {};
KnightMath M = {};
ParticleManager Particles = {};
Camera TheCamera = {};
Vector2 FieldSize = {};

#ifdef _DEBUG
int main()
#else
int WinMain()
#endif
{
	SetTraceLogLevel(LOG_ERROR);

	static Game game;

	int windowHeight = 960; //height
	int windowWidth = 1280; //width

	InitWindow(windowWidth, windowHeight, "Defender The Remake - Alpha 1.1");
	InitAudioDevice();

	Image icon = LoadImage("icon.png");
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	SetWindowIcon(icon);

	if (IsWindowState(FLAG_VSYNC_HINT)) ClearWindowState(FLAG_VSYNC_HINT);
	glfwSwapInterval(0);
	SetTargetFPS(120);

	Particles.Initialize();
	Particles.SetManagers(EM);

	// Define the camera to look into our 3D world
	// Camera position
	TheCamera.position = { 0.0f, 0.0f, -500.0f };
	// Camera looking at point
	TheCamera.target = { 0.0f, 0.0f, 0.0f };
	// Camera up vector (rotation towards target)
	TheCamera.up = { 0.0f, -1.0f, 0.0f };
	//Camera/World space is screen space.
	TheCamera.fovy = (float)GetScreenHeight();
	// Camera mode type
	TheCamera.projection = CAMERA_ORTHOGRAPHIC;
	// The Managers needs a reference to The Camera
	EM.SetCamera(TheCamera);

	game.Initialize();
	EM.Initialize();
	game.Load();
	game.BeginRun();
	EM.BeginRun();
	Particles.BeginRun();

	while (!WindowShouldClose())
	{
		game.Input();
		float deltaTime = GetFrameTime();

#if _DEBUG
		if (deltaTime > 0.05f) deltaTime = 0.05f;
#endif

		game.Update(deltaTime);

		if (game.Logic->State != GameState::Pause &&
			game.Logic->State != GameState::GameOver)
		{
			EM.Input();

			float deltaTime = GetFrameTime();

			EM.FixedUpdate(deltaTime);
			EM.AlwaysUpdate(deltaTime);
			EM.Update(deltaTime);
		}

		BeginDrawing();
		ClearBackground({ 8, 2, 16, 100 });
		BeginMode3D(TheCamera);
		game.Draw3D();
		EM.Draw3D();
		EndMode3D();
		game.Draw2D();
		EM.Draw2D();

#ifdef _DEBUG
		DrawFPS(5, 5);
#endif

		EndDrawing();
	}

	UnloadImage(icon);

	CloseWindow();
	CloseAudioDevice();
}