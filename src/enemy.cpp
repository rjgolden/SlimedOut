#include "enemy.h"
#include <cmath>

// Default and Destructor
Enemy::Enemy(){
    m_animationTextures[0] = LoadTexture("src/resources/hoodyIdleAnimation.png");
    m_animationTextures[1] = LoadTexture("src/resources/hoodyRunAnimation.png");
    m_animationTextures[2] = LoadTexture("src/resources/hoodyRunAnimation2.png");

    m_animationRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };
    m_hitboxRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };

    m_currentFrame = 0; // Starting frame
    m_frameCount = 6; // Number of frames in the idle animation
    m_runningTime = 0.0f;
    m_updateTime = 1.0f / 12.0f;

    // position and speed
    m_positionX = rand() % 900; 
    m_positionY = rand() % 400; 
    m_enemySpeed = 2.0;
}
Enemy::~Enemy() {
    UnloadTexture(m_animationTextures[0]);
    UnloadTexture(m_animationTextures[1]);
    UnloadTexture(m_animationTextures[2]);
}

// 1 animation enemy
Enemy::Enemy(const char* filePath, int frameCount){

    // Load textures
    m_animationTextures[0] = LoadTexture(filePath); 

    // Rectangles 
    m_animationRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height }; 
    m_hitboxRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };

    // frame stuff
    m_currentFrame = 0; // Starting frame
    m_frameCount = frameCount; // Number of frames in the idle animation
    m_runningTime = 0.0f; 
    m_updateTime = 1.0f / 12.0f; 

    // position and speed
    m_positionX = rand() % 900; 
    m_positionY = rand() % 400; 
    m_enemySpeed = 2.0;
}

// Multi frame animation enemy
Enemy::Enemy(const char* filePath, const char* filePath2, const char* filePath3, int frameCount) {
    // Load textures
    m_animationTextures[0] = LoadTexture(filePath); 
    m_animationTextures[1] = LoadTexture(filePath2);
    m_animationTextures[2] = LoadTexture(filePath3);

    // Rectangles 
    m_animationRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height }; 
    m_hitboxRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };

    // frame stuff
    m_currentFrame = 0; // Starting frame
    m_frameCount = frameCount; // Number of frames in the idle animation
    m_runningTime = 0.0f; 
    m_updateTime = 1.0f / 12.0f; 

    // position and speed
    m_positionX = rand() % 900; 
    m_positionY = rand() % 400; 
    m_enemySpeed = 2.0;
}

void Enemy::chasePlayer(float playerX, float playerY){
    float dx = playerX - m_positionX;
    float dy = playerY - m_positionY;
    float length = sqrt(dx*dx + dy*dy);
    if (length > 0) {
        dx /= length;
        dy /= length;
    }
    m_positionX += dx * m_enemySpeed;
    m_positionY += dy * m_enemySpeed;
}

void Enemy::setEnemySpeed(float speed){
    m_enemySpeed = speed;
}