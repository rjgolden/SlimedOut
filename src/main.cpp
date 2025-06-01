#include <raylib.h>
#include <cstdlib>
#include <iostream> 
#include <ctime>
#include <cmath>
#include "animation.h"

static float exponent = 1.0f;                 // Audio exponentiation value
static float averageVolume[400] = { 0.0f };   // Average volume history
static float volume = 0.5f;  // Volume multiplier (0.0 = mute, 1.0 = original volume)

//------------------------------------------------------------------------------------
// Audio processing function
//------------------------------------------------------------------------------------
void ProcessAudio(void *buffer, unsigned int frames)
{
    float *samples = (float *)buffer;
    float average = 0.0f;

    for (unsigned int frame = 0; frame < frames; frame++)
    {
        float *left = &samples[frame * 2 + 0];
        float *right = &samples[frame * 2 + 1];

        // Apply distortion
        *left = powf(fabsf(*left), exponent) * ((*left < 0.0f) ? -1.0f : 1.0f);
        *right = powf(fabsf(*right), exponent) * ((*right < 0.0f) ? -1.0f : 1.0f);

        // Apply fixed volume scaling
        *left *= volume;
        *right *= volume;

        average += (fabsf(*left) + fabsf(*right)) / frames;
    }

    for (int i = 0; i < 399; i++) averageVolume[i] = averageVolume[i + 1];
    averageVolume[399] = average;
}

int main()
{   
    // Screen setup
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 500, "Texture Test");
    SetWindowMinSize(1000, 500);
    SetTargetFPS(60);
    SetWindowIcon(LoadImage("src/resources/hoodyIdleAnimation.png"));
    SetTargetFPS(60);

    // Audio setup
    InitAudioDevice();
    AttachAudioMixedProcessor(ProcessAudio);
    Music music = LoadMusicStream("src/resources/03-ye-the_heil_symphony.mp3");
    Sound sound = LoadSound("src/resources/coin-pickup-98269.mp3");
    Sound hitSound = LoadSound("src/resources/hitSound.mp3");
    Sound gameOverSound = LoadSound("src/resources/gameOverSound.mp3"); 
    Sound powerUpSound = LoadSound("src/resources/powerUpSound.mp3");
    //PlayMusicStream(music);

    // Game flow variables
    int screen = 0;
    int score = 0;
    int lives = 3;

    // Random spaces for collectables
    srand(time(0));
    float collectableX = rand() % 900; 
    float collectableY = rand() % 400;

    // Control enemy position
    float enemyX = (rand() % 900);
    float enemyY = (rand() % 400);
    float speed = 2.0;

    // Power-up variables
    bool spawnPowerUp = false;
    bool powerUpSpawned = false;
    float powerUpY = (rand() % 900); 
    float powerUpX = (rand() % 900);

    // Load all textures
    Texture2D squishy = LoadTexture("src/resources/Squishy.png");        
    Texture2D gameScreen = LoadTexture("src/resources/gameScreen.png"); 
    Texture2D collectable = LoadTexture("src/resources/collectable.png");     
    Texture2D enemy = LoadTexture("src/resources/enemy.png");  
    Texture2D powerUp = LoadTexture("src/resources/powerUp.png");

    // Create hit boxes for player and collectables
    Rectangle collectableRect = {collectableX, collectableY, (float)collectable.width, (float)collectable.height};
    Rectangle enemyRect = {enemyX, enemyY, (float)enemy.width, (float)enemy.height};
    Rectangle powerUpRect = {powerUpX, powerUpY, (float)powerUp.width, (float)powerUp.height};

    // Load animations
    Animation hoodyAnimation("src/resources/hoodyIdleAnimation.png", "src/resources/hoodyRunAnimation.png", 6);
    Animation gemstoneAnimation("src/resources/hoodyGemAnimation.png", "src/resources/hoodyGemAnimation.png", 6);
    Animation enemyAnimation("src/resources/hoodyGuyEnemyAnimation.png", "src/resources/hoodyGuyEnemyAnimation.png", 6);
    system("cls");

    // Main Game Loop
    while (!WindowShouldClose())
    {   
        UpdateMusicStream(music);   // Update music buffer with new stream data

        if(screen == 0){ // splash screen 
            BeginDrawing();
                DrawTexture(gameScreen, 0, 0, WHITE);
                DrawText("Texture Test", 225, 100, 80, BLACK);
                DrawText("Press Enter to Start", 280, 300, 40, BLACK);
                if(IsKeyPressed(KEY_ENTER)) screen = 1;
                if(IsKeyPressed(KEY_UP)) ToggleFullscreen();
                if(IsKeyPressed(KEY_DOWN)) screen = 3;
            EndDrawing();
        }

        else if (screen == 1){ // game screen
            // Update hit boxes for player and collectables
            collectableRect.x = (float)collectableX;
            collectableRect.y = (float)collectableY;
            enemyRect.x = (float)enemyX;
            enemyRect.y = (float)enemyY;
            powerUpRect.x = (float)powerUpX;
            powerUpRect.y = (float)powerUpY;
            
            if(IsKeyPressed(KEY_F)) ToggleFullscreen();   

            float dx = hoodyAnimation.getPositionX() - enemyX;
            float dy = hoodyAnimation.getPositionY() - enemyY;
            float length = sqrt(dx*dx + dy*dy);
            if (length > 0) {
                dx /= length;
                dy /= length;
            }
            enemyX += dx * speed;
            enemyY += dy * speed;

            if(score >= 5 && !powerUpSpawned){
                spawnPowerUp = true;
                powerUpX = rand() % 900;
                powerUpY = rand() % 400;
                powerUpSpawned = true;
                speed = 5.0f;
            }

            if(CheckCollisionRecs(hoodyAnimation.getAnimationRect(), collectableRect)){
                system("cls");
                PlaySound(sound);
                score++;
                std::cout << "Score: " << score << "\n";
                collectableX = rand() % 900; 
                collectableY = rand() % 400;
            }   

            if(CheckCollisionRecs(hoodyAnimation.getAnimationRect(), enemyRect)){
                system("cls");
                PlaySound(hitSound);
                lives-=1;
                std::cout << "Lives: " << lives << "\n";
                enemyX = hoodyAnimation.getPositionX() + (rand() % 900);
                enemyY = hoodyAnimation.getPositionY() + (rand() % 400);
            }   

            if(CheckCollisionRecs(hoodyAnimation.getAnimationRect(), powerUpRect) && spawnPowerUp){
                system("cls");
                PlaySound(powerUpSound);
                std::cout << "Speed Up Collected! Speed: \n";
                hoodyAnimation.setPlayerSpeed(7.0f);
                powerUpY = 1000;
                powerUpX = 1000;
                spawnPowerUp = false;
            }  

            if(lives == 0){
                screen = 2;
            }

            BeginDrawing();
                DrawTexture(gameScreen, 0, 0, WHITE);
                DrawText(TextFormat("Score: %02i", score), 380, 30, 40, BLACK);
                DrawText(TextFormat("Lives: %i", lives), 400, 440, 40, BLACK);
                hoodyAnimation.Update();
                gemstoneAnimation.drawSprite();
                gemstoneAnimation.animateSprite();
                enemyAnimation.drawSprite();
                enemyAnimation.animateSprite();
                //DrawTexture(squishy, playerX, playerY, WHITE);
                DrawTexture(collectable, collectableX, collectableY, WHITE);
                DrawTexture(enemy, enemyX, enemyY, WHITE);
                if(spawnPowerUp){
                    DrawTexture(powerUp, powerUpX, powerUpY, WHITE);
                    //DrawRectangleLines(powerUpX, powerUpY, powerUp.width, powerUp.height, RED);
                }
                // Hitboxes
                //DrawRectangleLines(collectableX, collectableY, collectable.width, collectable.height, RED);
                //DrawRectangleLines(playerX, playerY, squishy.width, squishy.height, RED);
                //DrawRectangleLines(enemyX, enemyY, enemy.width, enemy.height, RED);
            EndDrawing();
        }

        else if(screen == 2){ // game over screen 
            PlaySound(gameOverSound);
            if(lives == 0) std::cout << "You fuckin suck! Try again loser...\n";
            lives= 3;
            BeginDrawing();
                DrawTexture(gameScreen, 0, 0, WHITE);
                DrawText("GAME OVER!", 270, 100, 80, BLACK);
                DrawText("Press Enter to restart!", 260, 300, 40, BLACK);
                if(IsKeyPressed(KEY_ENTER)){
                    //re initialize all data
                    score = 0;
                    hoodyAnimation.setPlayerSpeed(5.0f);
                    speed = 2.0f;
                    lives = 3;
                    spawnPowerUp = false;
                    powerUpSpawned = false;
                    collectableX = rand() % 900; 
                    collectableY = rand() % 400;
                    enemyX = rand() % 900;
                    enemyY = rand() % 400;
                    powerUpX = rand() % 900;
                    powerUpY = rand() % 400;
                    hoodyAnimation.setPlayerSpeed(5.0f);
                    hoodyAnimation.setPosition(220.0f, 300.0f);
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
    UnloadTexture(powerUp);

    // Unload sounds
    UnloadMusicStream(music);
    UnloadSound(sound);
    UnloadSound(hitSound);
    UnloadSound(gameOverSound);
    UnloadSound(powerUpSound);
    DetachAudioMixedProcessor(ProcessAudio);
    CloseAudioDevice(); // Close audio device

    // Closes window and ends program
    CloseWindow();
}
    

