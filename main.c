// main.c
#include "includes/raylib.h"
#include <stdlib.h>
#include <stdio.h>

#define GRID_W 16
#define GRID_H 8

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib - Radiance demo (debug: render input texture)");
    SetTargetFPS(60);

    // create a tiny cell texture (GRID_W x GRID_H). emitters are bright colors, blockers = black
    Image cellsImg = GenImageColor(GRID_W, GRID_H, RED);
    ImageDrawPixel(&cellsImg, 2, 3, (Color){255, 200, 80, 255});
    ImageDrawPixel(&cellsImg, 3, 3, (Color){255, 200, 80, 255});
    ImageDrawPixel(&cellsImg, 12, 4, (Color){80, 220, 255, 255});
    ImageDrawPixel(&cellsImg, 12, 5, (Color){80, 220, 255, 255});

    Texture2D cellsTex = LoadTextureFromImage(cellsImg);
    // use point filter for crisp debug view, bilinear if you want smooth upsampling
    SetTextureFilter(cellsTex, TEXTURE_FILTER_POINT);
    SetTextureWrap(cellsTex, TEXTURE_WRAP_CLAMP);

    // load vertex shader + simple fragment shader (renders input texture)
    Shader shader = LoadShader("radiance.vs.glsl", "radiance.fs.glsl");

    // uniform locations (we only need the texture sampler here)
    int locCells = GetShaderLocation(shader, "uCells");

    // bind texture to shader (bind texture unit 0)
    SetShaderValueTexture(shader, locCells, cellsTex);
    int texUnit = 0;
    SetShaderValue(shader, locCells, &texUnit, SHADER_UNIFORM_INT);

    // demo state (keep the 3s door timer)
    bool doorOpen = false;
    double timer = 0.0;

    while (!WindowShouldClose())
    {
        timer += GetFrameTime();

        if (timer > 3.0)
        {
            timer = 0.0;
            doorOpen = !doorOpen;
            if (doorOpen)
            {
                // open: add a small corridor of emitter pixels
                ImageDrawPixel(&cellsImg, 6, 0, (Color){200, 160, 120, 255});
                ImageDrawPixel(&cellsImg, 6, 1, (Color){200, 160, 120, 255});
                ImageDrawPixel(&cellsImg, 6, 2, (Color){200, 160, 120, 255});
            }
            else
            {
                // close: block those cells
                ImageDrawPixel(&cellsImg, 6, 0, BLACK);
                ImageDrawPixel(&cellsImg, 6, 1, BLACK);
                ImageDrawPixel(&cellsImg, 6, 2, BLACK);
            }
            // push update to GPU texture
            UpdateTexture(cellsTex, cellsImg.data);
            // rebind to shader (raylib convenience)
            SetShaderValueTexture(shader, locCells, cellsTex);
        }
        // ensure shader sees the latest texture binding each frame
        SetShaderValueTexture(shader, locCells, cellsTex);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginShaderMode(shader);
        Rectangle dest = {screenWidth / 2 - 200.0f, screenHeight / 2 - 150.0f, 400.0f, 300.0f};
        Rectangle src = {0.0f, 0.0f, (float)cellsTex.width, (float)cellsTex.height};
        // origin at top-left (0,0), no rotation
        DrawTexturePro(cellsTex, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        EndShaderMode();

        // debug: show the raw small cell texture (upscaled) so you can compare
        DrawTextureEx(cellsTex, (Vector2){10, 60}, 0.0f, 10.0f, WHITE);

        DrawText("Debug: shader now just renders the input cell texture", 10, 10, 14, DARKGRAY);
        DrawText("Door toggles every 3s (cells update)", 10, 30, 14, DARKGRAY);

        EndDrawing();
    }

    UnloadShader(shader);
    UnloadTexture(cellsTex);
    UnloadImage(cellsImg);

    CloseWindow();
    return 0;
}