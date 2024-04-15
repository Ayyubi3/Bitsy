#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"

#include "Block.h"
#include "Player.h"
#include "Chunk.h"

#include "test.h"

Chunk Chnk = {0};
Player *playerptr;

int main(void)
{

    // Setup
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");
    DisableCursor();
    SetTargetFPS(60);

    Texture2D texture = block_texture_create();

    playerptr = player_create();

    // test world

    test_world(&Chnk);

    Model model = LoadModelFromMesh(Block_Draw());

    while (!WindowShouldClose())
    {

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(playerptr->player_camera);

            UpdateCamera(&(playerptr->player_camera), CAMERA_FREE);

            DrawPlane((Vector3){0.0f, -10.0f, 0.0f}, (Vector2){32.0f, 32.0f}, GREEN);

            for (int i = 0; i < CHUNK_SIZE; i++)
            {
                for (int j = 0; j < CHUNK_SIZE; j++)
                {
                    for (int k = 0; k < CHUNK_SIZE; k++)
                    {
                        if (Chnk.Blocks[i][j][k].BlockID > 0)
                        {
                            DrawModel(model, (Vector3){i, j, k}, 1.0f, YELLOW);
                        }
                    }
                }
            }

            EndMode3D();
        }

        DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", playerptr->player_camera.position.x, playerptr->player_camera.position.y, playerptr->player_camera.position.z), 610, 60, 10, BLACK);
        DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", playerptr->player_camera.target.x, playerptr->player_camera.target.y, playerptr->player_camera.target.z), 610, 75, 10, BLACK);
        DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", playerptr->player_camera.up.x, playerptr->player_camera.up.y, playerptr->player_camera.up.z), 610, 90, 10, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
