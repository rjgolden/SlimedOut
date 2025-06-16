#include <raylib.h>
#include <iostream>
#include <vector> 
#include "animation.h"
#include "player.h"
#include "enemy.h"
#include "soundSystem.h"

struct Particle { 
    Vector2 pos, vel; 
    float life, rotation, scale; 
    Color tint;
    int textureIndex; // Which texture to use
};

std::vector<Texture2D> particleTextures;
std::vector<Particle> particles(100);

void Explode(Vector2 pos, Color tint, int textureIndex = 0) {
    for (int i = 0; i < 10; ++i) {
        for (auto& p : particles) {
            if (p.life <= 0) {
                p = {pos, {(float)GetRandomValue(-100,100), (float)GetRandomValue(-100,100)}, 
                     1.0f, (float)GetRandomValue(0, 360), 1.0f, tint, textureIndex};
                break;
            }
        }
    }
}

void updateParticles() {
    for (auto& p : particles) {
        if (p.life > 0) {
            p.pos.x += p.vel.x * GetFrameTime();
            p.pos.y += p.vel.y * GetFrameTime();
            p.life -= GetFrameTime();
            p.scale = p.life; // Scale based on life
            if (p.life <= 0) {
                p.tint.a = 0; // Make transparent when dead
            }

            Texture2D& tex = particleTextures[p.textureIndex];
            Rectangle source = {0, 0, (float)tex.width, (float)tex.height};
            Rectangle dest = {p.pos.x, p.pos.y, tex.width * p.scale, tex.height * p.scale};
            Vector2 origin = {tex.width * p.scale * 0.5f, tex.height * p.scale * 0.5f};
            
            DrawTexturePro(tex, source, dest, origin, p.rotation, p.tint);
        }
    }
}

int main()
{   
    // Screen setup
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(640, 360, "Tower Defense Game");
    SetWindowMinSize(640, 360);
    SetTargetFPS(60);
    SetWindowIcon(LoadImage("src/resources/Textures/building.png"));
    HideCursor();                               
 
    // Audio setup
    SoundSystem soundSystem; // Initialize sound system
    InitAudioDevice();
    AttachAudioMixedProcessor(SoundSystem::ProcessAudio); // Attach audio processor

    // Load music and sounds
    Sound collectableSound = LoadSound("src/resources/Sounds/coin-pickup-98269.mp3");
    Sound hitSound = LoadSound("src/resources/Sounds/hitSound.mp3");
    Sound gameOverSound = LoadSound("src/resources/Sounds/gameOverSound.mp3"); 
    Sound powerUpSound = LoadSound("src/resources/Sounds/powerUpSound.mp3");
    Sound enemyHurtSound = LoadSound("src/resources/Sounds/enemyHurtSound.mp3");
    Sound enemyHurtSound2 = LoadSound("src/resources/Sounds/enemyHurtSound.mp3");
    //Music music = LoadMusicStream("src/resources/03-ye-the_heil_symphony.mp3");
    //PlayMusicStream(music);
    
    // Load all textures        
    Texture2D gameScreen = LoadTexture("src/resources/Textures/gameScreen.png"); 
    Texture2D collectable = LoadTexture("src/resources/Textures/collectable.png");
    Texture2D building = LoadTexture("src/resources/Textures/building.png");    
    
    // Load different textures at startup
    particleTextures.push_back(LoadTexture("src/resources/Textures/blood1.png"));
    particleTextures.push_back(LoadTexture("src/resources/Textures/blood2.png"));
    particleTextures.push_back(LoadTexture("src/resources/Textures/blood3.png"));

    // Load animations
    //Player hoodyAnimation("src/resources/hoodyIdleAnimation.png", "src/resources/hoodyRunAnimation.png", "src/resources/hoodyRunAnimation2.png", 6);
    Animation gemstoneAnimation("src/resources/Animations/hoodyGemAnimation.png", 6, 500.0f, 500.0f);
    Animation fireAnimation("src/resources/Animations/fireSpriteAnimation.png", 6, rand() % 540, rand() % 360);
    Enemy enemyAnimation("src/resources/Animations/hoodyGuyEnemyAnimation.png", 6);
    Player captainAnimation("src/resources/Animations/Captain-Idle-Sheet.png", 
                            "src/resources/Animations/Captain-RunRight-Sheet.png", 
                            "src/resources/Animations/Captain-RunLeft-Sheet.png", 
                            "src/resources/Animations/Captain-AttackLeft-Sheet.png", 
                            "src/resources/Animations/Captain-AttackRight-Sheet.png", 10);

    // Game flow variables
    int screen = 0;
    int score = 0;
    int enemiesKilled = 0;
    bool paused = false;
    bool fullscreen = false;
    float gameTime = 0.0f;
    float pausedTime = 0.0f;
    float scale = 1.0f;

    // Random spaces for collectables
    srand(time(0));
    float collectableX = rand() % 540; 
    float collectableY = rand() % 360;

    // Power-up variables
    bool spawnPowerUp = false;
    bool powerUpSpawned = false;


    // Create hit boxes for player and collectables
    Rectangle collectableRect = {collectableX, collectableY, (float)collectable.width, (float)collectable.height};

    // Main Game Loop
    while (!WindowShouldClose())
    {   

        if(screen == 0){ // splash screen 
            BeginDrawing();
                DrawTexture(gameScreen, 0, 0, WHITE);
                DrawText("Tower Defense: Roguelite", 37, 100, 80, BLACK);
                DrawText("Press Enter to Start", 100, 250, 40, BLACK);
                if(IsKeyPressed(KEY_ENTER)) screen = 1;
            EndDrawing();
        }

        else if (screen == 1){ // game screen
            if(IsKeyPressed(KEY_P)) {
                paused = !paused; // Toggle pause state
            }
            if(!paused) {
                //UpdateMusicStream(music);   // Update music buffer with new stream data
                collectableRect.x = (float)collectableX;
                collectableRect.y = (float)collectableY;

                if(score >= 5 && !powerUpSpawned){
                    spawnPowerUp = true;
                    gemstoneAnimation.setPosition(rand() % 540, rand() % 360);
                    powerUpSpawned = true;
                    enemyAnimation.setEnemySpeed(5.0f);
                }

                if(CheckCollisionRecs(captainAnimation.getHitboxRect(), collectableRect)){
                    system("cls");
                    PlaySound(collectableSound);
                    score++;
                    std::cout << "Score: " << score << "\n";
                    collectableX = rand() % 540; 
                    collectableY = rand() % 360;
                }   

                if(CheckCollisionRecs(captainAnimation.getHitboxRect(), enemyAnimation.getHitboxRect())){
                    system("cls");
                    PlaySound(hitSound);
                    captainAnimation.takeDamage(5);
                    std::cout << "Current Health: " << captainAnimation.getHealth() << "\n";
                }   

                if(CheckCollisionRecs(captainAnimation.getHitboxRect(), gemstoneAnimation.getHitboxRect()) && spawnPowerUp){
                    system("cls");
                    PlaySound(powerUpSound);
                    std::cout << "Speed Up Collected! Speed: \n";
                    captainAnimation.setPlayerSpeed(7.0f);
                    gemstoneAnimation.setPosition(rand() % 540, rand() % 360);
                    spawnPowerUp = false;
                }  

                if(CheckCollisionRecs(captainAnimation.getAttackRect(),  enemyAnimation.getHitboxRect())){
                    system("cls");
                    int randomSound = rand() % 2; // Randomly choose between two sounds
                    if(randomSound == 0) {PlaySound(enemyHurtSound);}
                    else {PlaySound(enemyHurtSound2);}
                    enemyAnimation.takeDamage(25); 
                    std::cout << "Enemies health: " << enemyAnimation.getHealth() << "\n"; 
                    if(enemyAnimation.getHealth() <= 0){
                        Explode(enemyAnimation.getPosition(), RED, rand() % particleTextures.size()); // Randomly choose a texture index
                        std::cout << "Enemies killed: " << ++enemiesKilled << "\n"; 
                        enemyAnimation.setHealth(100); 
                        enemyAnimation.setPosition(rand() % 540, rand() % 360); 
                    }
                }  

                if(captainAnimation.getHealth() == 0){
                    screen = 2;
                }
            }
            BeginDrawing();

                DrawTextureEx(gameScreen, {0.0f,0.0f}, 0.0f, scale, WHITE);
                DrawText(TextFormat("Score: %02i", score), 200, 30, 40, BLACK);
                DrawText(TextFormat("Time Elapsed: %02f", gameTime), 200, 10, 20, BLACK);
                DrawTexture(collectable, collectableX, collectableY, WHITE);
                DrawTexture(building, 0.0f, 160.0f, WHITE);
                if(IsKeyPressed(KEY_F)) {
                    fullscreen = !fullscreen;
                    if(fullscreen){
                        SetWindowPosition(0, 0);
                        SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0));
                        scale = 3.0f;
                    }
                    else{
                        SetWindowPosition(GetMonitorWidth(0)/3, GetMonitorHeight(0)/3);
                        SetWindowSize(640, 360);
                        scale = 1.0f;
                    }
                }
                if(paused){
                    ShowCursor(); // Show cursor when paused
                    captainAnimation.drawSprite();
                    captainAnimation.drawHealthBar();
                    enemyAnimation.drawSprite();
                    enemyAnimation.drawHealthBar();
                    fireAnimation.drawSprite();
                    if(spawnPowerUp){
                        gemstoneAnimation.drawSprite();
                    }
                    DrawText("Game Paused", 200, 150, 40, RED);
                    DrawText("Press P to resume", 220, 200, 20, RED);
                    pausedTime = GetTime() - gameTime;
                }

                else{
                    HideCursor(); // Hide cursor when not paused
                    gameTime = GetTime() - pausedTime;
                    captainAnimation.updateSprite();
                    enemyAnimation.updateSprite();
                    fireAnimation.updateSprite();

                    //temp
                    updateParticles(); 

                    enemyAnimation.chasePlayer(captainAnimation.getPositionX(), captainAnimation.getPositionY());
                    if(spawnPowerUp){
                    gemstoneAnimation.updateSprite();
                    }
                }

            EndDrawing();
        }

        else if(screen == 2){ // game over screen 
            PlaySound(gameOverSound);
            BeginDrawing();
                DrawTexture(gameScreen, 0, 0, WHITE);
                DrawText("GAME OVER!", 130, 100, 80, BLACK);
                DrawText("Press Enter to restart!", 100, 300, 40, BLACK);
                if(IsKeyPressed(KEY_ENTER)){
                    //re initialize all data
                    score = 0;
                    enemyAnimation.setEnemySpeed(2.0f);
                    spawnPowerUp = false;
                    powerUpSpawned = false;
                    collectableX = rand() % 540; 
                    collectableY = rand() % 360;
                    enemyAnimation.setPosition((rand() % 540) , (rand() % 360));
                    gemstoneAnimation.setPosition(rand() % 540, rand() % 360);
                    captainAnimation.setPlayerSpeed(3.0f);
                    captainAnimation.setPosition(220.0f, 300.0f);     
                    captainAnimation.setHealth(100);          
                    system("cls"); 
                    screen = 0;
                }
            EndDrawing();
        }
    }
        
    // Closes window and ends program
    CloseWindow();
}
    

