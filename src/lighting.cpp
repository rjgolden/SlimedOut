#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <iostream>
#include "animation.h"

// Constants
//--------------------------------------------------------------------------------------
const int screenWidth = 128;
const int screenHeight = 128;
const int scaleFactor = 3;
//--------------------------------------------------------------------------------------

int withinRect(int tx, int ty, int x, int y, int w, int h){
    return (tx > x) && (tx < x + w) && (ty > y) && (ty < y + h);
}

void PixRect(int x, int y, int w, int h, Color c){
    DrawRectangle(scaleFactor * x, scaleFactor * y, scaleFactor * w, scaleFactor * h, c);
}

int main(void){
    // Initialization
    //--------------------------------------------------------------------------------------
    int clock = 0;

    InitWindow(screenWidth * scaleFactor, screenHeight * scaleFactor, "Real Pixel Lighting");

    SetTargetFPS(60);         // Set our game framerate
    //--------------------------------------------------------------------------------------
    Animation hoodyAnimation("src/resources/hoodyIdleAnimation.png", "src/resources/hoodyRunAnimation.png", 6);
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        clock ++;
        float fps = GetFPS();
        // Draw 
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(WHITE); 
        int mouseX = GetMouseX() / scaleFactor;
        int mouseY = GetMouseY() / scaleFactor;
        //std::cout << "Mouse Position: " << mouseX << ", " << mouseY << std::endl;
        int RW = 10;
        int RH = 10;
        int RX = mouseX - RW / 2;
        int RY = mouseY - RH / 2;

        for (int x = 0; x < screenWidth; x ++){
            for (int y = 0; y < screenHeight; y ++){
                Vector2 lightSrc = CLITERAL(Vector2){screenWidth/2, screenHeight/2};
                Vector2 screenPos = CLITERAL(Vector2){float(x),float(y)};

                const float flickerRate = 4;
                const float flickerAmp = .003;

                const float intensity = .4 + flickerAmp * sin(clock * flickerRate);
                const float range = .95;

                const float bounceRate = .9;
                const float gi = .3;

                float d = Vector2Distance(lightSrc, screenPos);
                unsigned char b = 255*intensity - d/range;
                if (b < 0) b = 0;

                float emission = 1;

                float samples = 100;
                for (int i=0; i<samples; i++){
                    float testX = screenWidth/2 + (x - screenWidth/2) * i/samples;
                    float testY = screenHeight/2 + (y - screenHeight/2) * i/samples;

                    if (withinRect(testX, testY, RX, RY, RW, RH)){
                    emission *= bounceRate;
                    }
                }

                b =  gi * b + (1 - gi) * b * emission;

                Color c = CLITERAL(Color){ b, b, b, 255 };
                // Pass it directly:
                PixRect(x, y, 1, 1, c );
            }
        }

        PixRect(mouseX, mouseY, 1, 1, BLUE);
        PixRect(RX, RY, RW, RH, RED);
        hoodyAnimation.Update();

        DrawText(TextFormat("FPS: %02f", fps), 10, 10, 20, WHITE);  
        EndDrawing();
    //----------------------------------------------------------------------------------
  }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();    // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}