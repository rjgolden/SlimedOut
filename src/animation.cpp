#include "animation.h"

// Default
Animation::Animation(){
    m_animationTextures[0] = LoadTexture("src/resources/hoodyIdleAnimation.png");
    m_animationRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };
    m_hitboxRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };
    m_currentFrame = 0; // Starting frame
    m_frameCount = 6; // Number of frames in the idle animation
    m_runningTime = 0.0f; 
    m_updateTime = 1.0f / 12.0f; 
    m_positionX = rand() % 540; 
    m_positionY = rand() % 360; 
}

// Destructor
Animation::~Animation(){
    UnloadTexture(m_animationTextures[0]);
}

// Meat and Potatoes
Animation::Animation(const char* filePath, int frameCount){

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
    m_positionX = rand() % 540; 
    m_positionY = rand() % 360; 
}

void Animation::animateSprite(){
    float deltaTime = GetFrameTime();
    m_runningTime += deltaTime;
    if (m_runningTime >= m_updateTime){
        m_runningTime = 0.0f;
        m_animationRect.x = (float)m_currentFrame * m_animationRect.width;
        m_currentFrame++;
        if (m_currentFrame > m_frameCount) m_currentFrame = 0;
    }
}

void Animation::drawSprite(){
    DrawTextureRec(m_animationTextures[0], m_animationRect, {m_positionX, m_positionY}, WHITE);
}

void Animation::drawHitbox(){
    DrawRectangleLines(m_positionX, m_positionY, m_hitboxRect.width, m_hitboxRect.height, RED);
}

void Animation::updateSprite(){
    animateSprite();
    drawSprite();
    drawHitbox();
}

// getters
float Animation::getPositionX() { 
    return m_positionX; 
}

float Animation::getPositionY() { 
    return m_positionY; 
}

float Animation::getWidth() { 
    return m_hitboxRect.width; 
}

float Animation::getHeight() { 
    return m_hitboxRect.height; 
}

Rectangle Animation::getHitboxRect() { 
    return {m_positionX, m_positionY, m_hitboxRect.width, m_hitboxRect.height}; 
}

// setters
void Animation::setPosition(float x, float y){
    m_positionX = x;
    m_positionY = y;
}
