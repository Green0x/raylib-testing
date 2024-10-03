/*******************************************************************************************
*
*   raylib [core] example - 3d camera first person
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "rcamera.h"
#include <iostream>

#define MAX_COLUMNS 20

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	const int fontSize = 30;
	const float speed = 1.0f;
	InitWindow(screenWidth, screenHeight, "Map viewer");

	// Define the camera to look into our 3d world (position, target, up vector)
	Camera camera = { 0 };
	camera.position = Vector3{ 0.0f, 2.0f, 4.0f };    // Camera position
	camera.target = Vector3{ 0.0f, 2.0f, 0.0f };      // Camera looking at point
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 60.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

	Ray ray = { 0 };                    // Picking line ray
	RayCollision collision = { 0 };     // Ray collision hit info
	// Generates some random columns
	float heights[MAX_COLUMNS] = { 0 };
	Vector3 positions[MAX_COLUMNS] = { 0 };
	Color colors[MAX_COLUMNS] = { 0 };

	for (int i = 0; i < MAX_COLUMNS; i++)
	{
		heights[i] = (float)GetRandomValue(1, 12);
		positions[i] = Vector3{ (float)GetRandomValue(-15, 15), heights[i] / 2.0f, (float)GetRandomValue(-15, 15) };
		colors[i] = Color{ (unsigned char)GetRandomValue(20, 255), (unsigned char)GetRandomValue(10, 55), 30, 255 };
	}

	Vector3 cubePosition = { 0.0f, 1.0f, 0.0f };
	Vector3 cubeSize = { 2.0f, 2.0f, 2.0f };
	Model model = LoadModel("resources/alliancebomb.gltf");
	Texture2D texture = LoadTexture("resources/ags_deck.png");
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);   // Set model bounds

	//DisableCursor();                    // Limit cursor to relative movement inside the window

	SetTargetFPS(120);                   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())        // Detect window close button or ESC key
	{

		if (IsKeyPressed(KEY_ONE))
		{
			TraceLog(LOG_ERROR, "AAA");
		}
		// Load new models/textures on drag&drop
		if (IsFileDropped())
		{
			FilePathList droppedFiles = LoadDroppedFiles();

			if (droppedFiles.count == 1) // Only support one file dropped
			{
				if (IsFileExtension(droppedFiles.paths[0], ".wmo"))       // Model file formats supported
				{
					UnloadModel(model);                         // Unload previous model
					model = LoadModel(droppedFiles.paths[0]);   // Load new model
					model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set current map diffuse texture

					//bounds = GetMeshBoundingBox(model.meshes[0]);

					// TODO: Move camera position from target enough distance to visualize model properly
				}
				else if (IsFileExtension(droppedFiles.paths[0], ".png"))  // Texture file formats supported
				{
					// Unload current model texture and load new one
					UnloadTexture(texture);
					texture = LoadTexture(droppedFiles.paths[0]);
					model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
				}
			}

			UnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (!collision.hit)
			{
				ray = GetScreenToWorldRay(GetMousePosition(), camera);
				
				// Check collision between ray and box
				collision = GetRayCollisionBox(ray,
					BoundingBox{
					Vector3 {
						cubePosition.x - cubeSize.x / 2, cubePosition.y - cubeSize.y / 2, cubePosition.z - cubeSize.z / 2
						},
						Vector3 {
						cubePosition.x + cubeSize.x / 2, cubePosition.y + cubeSize.y / 2, cubePosition.z + cubeSize.z / 2
					}
					});
			}
			else collision.hit = false;
		}


		// Update camera
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			//DisableCursor();

			UpdateCameraPro(&camera,
				Vector3{
					(IsKeyDown(KEY_W)) * speed -   // Move forward-backward
					(IsKeyDown(KEY_S)) * speed,
					(IsKeyDown(KEY_D)) * speed -   // Move right-left
					(IsKeyDown(KEY_A)) * speed,
					(IsKeyDown(KEY_Q)) * speed -   // Move up-down
					(IsKeyDown(KEY_E)) * speed
				},
				Vector3{
					GetMouseDelta().x * 0.1f,     // Rotation: yaw
					GetMouseDelta().y * 0.1f,     // Rotation: pitch
					0.0f                          // Rotation: roll
				},
				GetMouseWheelMove() * 0.0f);      // Move to target (zoom)
		}
		else { // Don't move camera around using mouse unless right clicking
			UpdateCameraPro(&camera,
				Vector3{
					(IsKeyDown(KEY_W)) * speed -   // Move forward-backward
					(IsKeyDown(KEY_S)) * speed,
					(IsKeyDown(KEY_D)) * speed -   // Move right-left
					(IsKeyDown(KEY_A)) * speed,
					(IsKeyDown(KEY_Q)) * speed -   // Move up-down
					(IsKeyDown(KEY_E)) * speed
				},
				Vector3{
					GetMouseDelta().x * 0.0f,     // Rotation: yaw
					GetMouseDelta().y * 0.0f,     // Rotation: pitch
					0.0f                          // Rotation: roll
				},
				GetMouseWheelMove() * 0.0f);      // Move to target (zoom)
		}

		//----------------------------------------------------------------------------------
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();


		ClearBackground(RAYWHITE);

		BeginMode3D(camera);
		//DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, GRAY);

		

		//DrawRay(ray, MAROON);
		//DrawGrid(10, 1.0f);
		DrawModel(model, cubePosition, 1.0f, WHITE);
		DrawPlane(Vector3{ 533.33333f / 2, 0.0f, 533.33333f / 2 }, Vector2{ 533.33333f, 533.33333f }, GREEN); // Draw ground
		DrawCube(Vector3{ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
		DrawCube(Vector3{ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
		DrawCube(Vector3{ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);
		DrawGrid(32, 33.33333f);// Draw a yellow wall


		//DrawCube(Vector3{ 10.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, RED);
		// Draw some cubes around
		for (int i = 0; i < MAX_COLUMNS; i++)
		{
			DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
			//DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
		}

		if (collision.hit)
		{
			//DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
			//DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, MAROON);
			//TraceLog(LOG_INFO, "")
			//std::cout << "collision" << std::endl;
			DrawCubeWires(cubePosition, cubeSize.x + 0.2f, cubeSize.y + 0.2f, cubeSize.z + 0.2f, GREEN);
		}


		EndMode3D();

		// Draw info boxes


		DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
		DrawRectangleLines(600, 5, 195, 100, BLUE);

		DrawText("Camera status:", 610, 15, fontSize, BLACK);
		DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, fontSize, BLACK);
		//DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, fontSize, BLACK);
		//DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, fontSize, BLACK);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}