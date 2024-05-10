#include "raylib.h"
#include "rcamera.h"
#include "gui/raygui.h"

#include "core/rlights.h"
#include "core/Debug.h"
#include "core/Utils.h"

#include "world/World.h"
#include "entity/Player.h"
#include "world/Chunk.h"
#include "core/Shader.h"
#include "world/ChunkSystem.h"

Config CFG = {
    .flyingSpeed = 0.2f,
    .mouseActive = 1,
    .mouseSensitivity = 0.005f,
    .fieldOfView = 95.0f,
};

Player *player;

int loadedChunksCount = 0;
Chunk *loadedChunks[2000];

Shader shader;
Light light;
Texture2D tex;
Model block;

void setup()
{
    player = MemAlloc(sizeof(Player));
    player_init(player);

    shader_init(&shader, &light, &tex);

    GuiLoadStyleDefault();

    block = LoadModelFromMesh(mesh_block());
}
int x = 1;

void update()
{
    chunkSystem_update(player, loadedChunks, &loadedChunksCount, shader, tex);
    player_update(player, loadedChunks, &loadedChunksCount, &CFG);
    shader_update(&shader, &light, player->camera.position);

    // Debug
    DrawSphere(player->rayCollision.point, 0.4f, BLUE);
    DrawSphere((Vector3){0.5, 0.5, 0.5}, 0.9f, PURPLE);
    DrawLine3D(player->rayCollision.point, Vector3Add(player->rayCollision.point, player->rayCollision.normal), PURPLE);
    DrawRay(player->ray, GREEN);

    TraceLog(LOG_DEBUG, "Target Block: %f, %f, %f", player->targetBlockPosInWorldSpace.x, player->targetBlockPosInWorldSpace.y, player->targetBlockPosInWorldSpace.z);

    DrawModel(block, player->targetBlockPosInWorldSpace, 1.0, YELLOW);
}
void ui()
{
    DrawText(TextFormat("pos: (%f, %f, %f)", player->camera.position.x, player->camera.position.y, player->camera.position.z), 0, 0, 20, BLACK);

    Vector3 pos1 = worldPositionToChunk(player->camera.position);
    DrawText(TextFormat("chunkPos: %f, %f, %f", pos1.x, pos1.y, pos1.z), 0, 20, 20, BLACK);

    if (player->targetChunkValid)
    {
        DrawText(TextFormat("BlockSpace: chunk:  %.2f, %.2f, %.2f / blockPos: %.2f, %.2f, %.2f", player->targetChunk->pos.x, player->targetChunk->pos.y, player->targetChunk->pos.z, player->targetBlockPosInChunkSpace.x, player->targetBlockPosInChunkSpace.y, player->targetBlockPosInChunkSpace.z), 0, 40, 20, BLACK);
    }

    DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 1, GREEN);
}

int main(void)
{
    InitWindow(1080, 720, "Bitsy");
    DisableCursor();
    SetTargetFPS(120);
    SetTraceLogLevel(LOG_ALL);

    setup();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(player->camera);

        update();

        EndMode3D();

        ui();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
