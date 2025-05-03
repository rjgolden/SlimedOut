#include <raylib.h>
#include <cstdlib>
#include <iostream> 
#include <ctime>
#include <cmath>
#define WIDTH 1000
#define HEIGHT 500

int main()
{
    InitWindow(WIDTH, HEIGHT, "Slimed Out!");
    SetTargetFPS(60);
    int screen = 0;
    int score = 0;
    int lives = 3;

    // Control player movement 
    int playerX = 200;
    int playerY = 400;

    // Random spaces for collectables
    srand(time(0));
    int collectableX = rand() % 900; 
    int collectableY = rand() % 400;

    // Control enemy movement
    int enemyX = rand() % 900;
    int enemyY = rand() % 400;
    int speed = 2;

    // Load all textures
    Texture2D squishy = LoadTexture("src/resources/Squishy.png");        
    Texture2D gameScreen = LoadTexture("src/resources/gameScreen.png"); 
    Texture2D collectable = LoadTexture("src/resources/collectable.png");     
    Texture2D enemy = LoadTexture("src/resources/enemy.png");  
    system("cls");

    // Create hit boxes for player and collectables
    Rectangle squishyRect = {(float)playerX, (float)playerY, (float)squishy.width, (float)squishy.height};
    Rectangle collectableRect = {(float)collectableX, (float)collectableY, (float)collectable.width, (float)collectable.height};
    Rectangle enemyRect = {(float)enemyX, (float)enemyY, (float)enemy.width, (float)enemy.height};

    // Main Game Loop
    while (!WindowShouldClose())
    {   

        if(screen == 0){ // splash screen 
            BeginDrawing();
                DrawTexture(gameScreen, 0, 0, WHITE);
                DrawText("Slimed Out!", 300, 100, 80, BLACK);
                DrawText("Press Enter to Start!", 280, 300, 40, BLACK);
                if(IsKeyPressed(KEY_ENTER)) screen = 1;
            EndDrawing();
        }

        else if (screen == 1){ // game screen
            // Update hit boxes for player and collectables
            squishyRect.x = (float)playerX;
            squishyRect.y = (float)playerY;
            collectableRect.x = (float)collectableX;
            collectableRect.y = (float)collectableY;
            enemyRect.x = (float)enemyX;
            enemyRect.y = (float)enemyY;

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) playerX +=5;
            else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) playerX -=5;
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) playerY-=5;
            else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) playerY+=5;

            float dx = playerX - enemyX;
            float dy = playerY - enemyY;
            float length = sqrt(dx*dx + dy*dy);
            if (length > 0) {
                dx /= length;
                dy /= length;
            }
            enemyX += dx * speed;
            enemyY += dy * speed;

            if(score > 10) speed = 5;
            else if(score < 10) speed = 2;
            
            if(CheckCollisionRecs(squishyRect, collectableRect)){
                std::cout << "+1!\n";
                score++;
                collectableX = rand() % 900; 
                collectableY = rand() % 400;
            }   

            if(CheckCollisionRecs(squishyRect, enemyRect)){
                std::cout << "Ouch!\n";
                lives-=1;
                enemyX = rand() % 900;
                enemyY = rand() % 400;
            }   

            if(lives == 0){
                screen = 2;
            }

            BeginDrawing();
                DrawTexture(gameScreen, 0, 0, WHITE);
                DrawText(TextFormat("Score: %02i", score), 380, 30, 40, BLACK);
                DrawText(TextFormat("Lives: %i", lives), 400, 440, 40, BLACK);
                DrawTexture(squishy, playerX, playerY, WHITE);
                DrawTexture(collectable, collectableX, collectableY, WHITE);
                DrawTexture(enemy, enemyX, enemyY, WHITE);

                // Hitboxes
                //DrawRectangleLines(collectableX, collectableY, collectable.width, collectable.height, RED);
                //DrawRectangleLines(playerX, playerY, squishy.width, squishy.height, RED);
                //DrawRectangleLines(enemyX, enemyY, enemy.width, enemy.height, RED);
            EndDrawing();
        }

        if(screen == 2){ // splash screen 
            BeginDrawing();
                DrawTexture(gameScreen, 0, 0, WHITE);
                DrawText("GAME OVER!", 270, 100, 80, BLACK);
                DrawText("Press Enter to restart!", 260, 300, 40, BLACK);
                if(IsKeyPressed(KEY_ENTER)){
                    score = 0;
                    lives = 3;
                    collectableX = rand() % 900; 
                    collectableY = rand() % 400;
                    enemyX = rand() % 900;
                    enemyY = rand() % 400;
                    std::cout << "Resetting Game...\n";
                    system("cls");
                    screen = 0;
                }
            EndDrawing();
        }
    }
        
    // UnloadTextures
    UnloadTexture(squishy);       
    UnloadTexture(gameScreen);      
    UnloadTexture(collectable);  
    UnloadTexture(enemy);

    // Closes window and ends program
    CloseWindow();
}
    

