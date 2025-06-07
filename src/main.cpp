#include <raylib.h>
#include <cstdlib>
#include <iostream> 
#include <ctime>
#include <cmath>
#include "animation.h"
#include "player.h"
#include "enemy.h"

static float exponent = 1.0f;                 // Audio exponentiation value
static float averageVolume[400] = { 0.0f };   // Average volume history
static float volume = 0.5f;  // Volume multiplier (0.0 = mute, 1.0 = original volume)

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
    InitWindow(640, 360, "Texture Test");
    SetWindowMinSize(640, 360);
    SetTargetFPS(60);
    SetWindowIcon(LoadImage("src/resources/powerUp.png"));

    // Audio setup
    InitAudioDevice();
    AttachAudioMixedProcessor(ProcessAudio);
    Music music = LoadMusicStream("src/resources/03-ye-the_heil_symphony.mp3");
    Sound sound = LoadSound("src/resources/coin-pickup-98269.mp3");
    Sound hitSound = LoadSound("src/resources/hitSound.mp3");
    Sound gameOverSound = LoadSound("src/resources/gameOverSound.mp3"); 
    Sound powerUpSound = LoadSound("src/resources/powerUpSound.mp3");
    Sound enemyHurtSound = LoadSound("src/resources/enemyHurtSound.mp3");
    Sound enemyHurtSound2 = LoadSound("src/resources/enemyHurtSound.mp3");
    //PlayMusicStream(music);

    // Game flow variables
    int screen = 0;
    int score = 0;
    int lives = 3;
    int enemiesKilled = 0;

    // Random spaces for collectables
    srand(time(0));
    float collectableX = rand() % 540; 
    float collectableY = rand() % 360;

    // Power-up variables
    bool spawnPowerUp = false;
    bool powerUpSpawned = false;

    // Load all textures
    Texture2D squishy = LoadTexture("src/resources/Squishy.png");        
    Texture2D gameScreen = LoadTexture("src/resources/gameScreen.png"); 
    Texture2D collectable = LoadTexture("src/resources/collectable.png");     
    Texture2D powerUp = LoadTexture("src/resources/powerUp.png");

    // Create hit boxes for player and collectables
    Rectangle collectableRect = {collectableX, collectableY, (float)collectable.width, (float)collectable.height};

    // Load animations
    Player hoodyAnimation("src/resources/hoodyIdleAnimation.png", "src/resources/hoodyRunAnimation.png", "src/resources/hoodyRunAnimation2.png", 6);
    Animation gemstoneAnimation("src/resources/hoodyGemAnimation.png", 6);
    Enemy enemyAnimation("src/resources/hoodyGuyEnemyAnimation.png", 6);
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

            if(IsKeyPressed(KEY_F)) ToggleFullscreen();   

            if(score >= 5 && !powerUpSpawned){
                spawnPowerUp = true;
                gemstoneAnimation.setPosition(rand() % 540, rand() % 360);
                powerUpSpawned = true;
                enemyAnimation.setEnemySpeed(5.0f);
            }

            if(CheckCollisionRecs(hoodyAnimation.getHitboxRect(), collectableRect)){
                system("cls");
                PlaySound(sound);
                score++;
                std::cout << "Score: " << score << "\n";
                collectableX = rand() % 540; 
                collectableY = rand() % 360;
            }   

            if(CheckCollisionRecs(hoodyAnimation.getHitboxRect(), enemyAnimation.getHitboxRect())){
                system("cls");
                PlaySound(hitSound);
                lives-=1;
                std::cout << "Lives: " << lives << "\n";
                enemyAnimation.setPosition(hoodyAnimation.getPositionX() + (rand() % 540), hoodyAnimation.getPositionY() + (rand() % 360));
            }   

            if(CheckCollisionRecs(hoodyAnimation.getHitboxRect(), gemstoneAnimation.getHitboxRect()) && spawnPowerUp){
                system("cls");
                PlaySound(powerUpSound);
                std::cout << "Speed Up Collected! Speed: \n";
                hoodyAnimation.setPlayerSpeed(7.0f);
                gemstoneAnimation.setPosition(rand() % 540, rand() % 360);
                spawnPowerUp = false;
            }  

            if(CheckCollisionRecs(hoodyAnimation.getAttackRect(),  enemyAnimation.getHitboxRect())){
                system("cls");

                int randomSound = rand() % 2; // Randomly choose between two sounds
                if(randomSound == 0) {PlaySound(enemyHurtSound);}
                else {PlaySound(enemyHurtSound2);}

                enemyAnimation.takeDamage(5.0f); 
                std::cout << "Enemies health: " << enemyAnimation.getHealth() << "\n"; 
                if(enemyAnimation.getHealth() <= 0){
                    std::cout << "Enemies killed: " << ++enemiesKilled << "\n"; 
                    enemyAnimation.setHealth(100); // Reset enemy health
                    enemyAnimation.setPosition(rand() % 540, rand() % 360); // Respawn enemy at random position
                }
            }  

            if(lives == 0){
                screen = 2;
            }

            BeginDrawing();
                DrawTexture(gameScreen, 0, 0, WHITE);
                DrawText(TextFormat("Score: %02i", score), 380, 30, 40, BLACK);
                DrawText(TextFormat("Lives: %i", lives), 400, 440, 40, BLACK);
                hoodyAnimation.updateSprite();
                enemyAnimation.updateSprite();
                enemyAnimation.chasePlayer(hoodyAnimation.getPositionX(), hoodyAnimation.getPositionY());
                DrawTexture(collectable, collectableX, collectableY, WHITE);
                if(spawnPowerUp){
                    gemstoneAnimation.updateSprite();
                }
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
                    enemyAnimation.setEnemySpeed(2.0f);
                    lives = 3;
                    spawnPowerUp = false;
                    powerUpSpawned = false;
                    collectableX = rand() % 540; 
                    collectableY = rand() % 360;
                    enemyAnimation.setPosition((rand() % 540) , (rand() % 360));
                    gemstoneAnimation.setPosition(rand() % 540, rand() % 360);
                    hoodyAnimation.setPlayerSpeed(3.0f);
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
    UnloadTexture(powerUp);

    // Unload animations
    hoodyAnimation.~Player();
    gemstoneAnimation.~Animation();
    enemyAnimation.~Enemy();

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
    

