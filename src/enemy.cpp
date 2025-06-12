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
    m_positionX = rand() % 540; 
    m_positionY = rand() % 360; 
    m_enemySpeed = 2.0;
    m_health = 100;
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
    m_enemyHurt = LoadTexture("src/resources/hoodyGuyEnemyHurt.png"); // Load enemy hurt texture

    // Rectangles 
    m_animationRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height }; 
    m_hitboxRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };
    m_healthBarRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, 10.0f }; // Health bar rectangle

    // frame stuff
    m_currentFrame = 0; // Starting frame
    m_frameCount = frameCount; // Number of frames in the idle animation
    m_runningTime = 0.0f; 
    m_updateTime = 1.0f / 12.0f; 

    // position and speed
    m_positionX = rand() % 540; 
    m_positionY = rand() % 360; 
    m_enemySpeed = 2.0;
    m_health = 100;
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
    m_healthBarRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, 10.0f }; // Health bar rectangle

    // frame stuff
    m_currentFrame = 0; // Starting frame
    m_frameCount = frameCount; // Number of frames in the idle animation
    m_runningTime = 0.0f; 
    m_updateTime = 1.0f / 12.0f; 

    // position and speed
    m_positionX = rand() % 900; 
    m_positionY = rand() % 400; 
    m_enemySpeed = 2.0;
    m_health = 100;
}

void Enemy::animateSprite(){
    float deltaTime = GetFrameTime();
    m_runningTime += deltaTime;
    if (m_runningTime >= m_updateTime){
        m_runningTime = 0.0f;
        m_animationRect.x = (float)m_currentFrame * m_animationRect.width;
        m_currentFrame++;
        if (m_currentFrame > m_frameCount) m_currentFrame = 0;
    }
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

int Enemy::getHealth() {
    return m_health;
}  

void Enemy::setHealth(int health) {
    m_health = health;
}

void Enemy::takeDamage(int damage) {
    m_hurtFrameActive = true; // Activate hurt frame
    m_health -= damage;
    if (m_health < 0) {
        m_health = 0; // Prevent negative health
    }
}

void Enemy::drawHealthBar(){
    // Draw background
    DrawRectangle(m_healthBarRect.x, m_healthBarRect.y, m_healthBarRect.width, m_healthBarRect.height, GRAY);
    
    // Calculate health bar width based on current health
    float healthWidth = (m_health / 100.0f) * m_healthBarRect.width;
    
    // Draw health bar
    DrawRectangle(m_healthBarRect.x, m_healthBarRect.y, healthWidth, m_healthBarRect.height, RED);
    DrawRectangleLines(m_healthBarRect.x, m_healthBarRect.y, m_healthBarRect.width, m_healthBarRect.height, BLACK);
    
}

void Enemy::updateSprite(){
    m_healthBarRect.x = m_positionX; 
    m_healthBarRect.y = m_positionY - 13.0f;
    if( m_hurtFrameActive) {
        drawHurtFrame();
        m_hurtFrameActive = false; // Reset hurt frame after drawing
    }
    else{
        animateSprite();
        drawSprite();
    }
    //drawHitbox();
    drawHealthBar();
}
    
void Enemy::drawHurtFrame() {
    DrawTexture(m_enemyHurt, m_positionX, m_positionY, WHITE);
}
