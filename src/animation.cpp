#include "animation.h"

// Default
Animation::Animation(){
    m_animationTextures[0] = LoadTexture("src/resources/hoodyIdleAnimation.png");
    m_animationTextures[1] = LoadTexture("src/resources/hoodyRunAnimation.png");
    m_animationTextures[2] = LoadTexture("src/resources/hoodyRunAnimation2.png");
    m_animationRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };
    m_hitboxRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };
    m_currentFrame = 0; // Starting frame
    m_frameCount = 6; // Number of frames in the idle animation
    m_runningTime = 0.0f; 
    m_updateTime = 1.0f / 12.0f; 
    m_idle = true; 
    m_positionX = rand() % 900; 
    m_positionY = rand() % 400; 
    m_playerSpeed = 3.0f;
    m_direction = 0;
    m_lastDirection = 0;
}

// Destructor
Animation::~Animation(){
    UnloadTexture(m_animationTextures[0]);
    UnloadTexture(m_animationTextures[1]);
}

// Meat and Potatoes
Animation::Animation(const char* filePath, const char* filePath2, int frameCount){

    // Load textures
    m_animationTextures[0] = LoadTexture(filePath); 
    m_animationTextures[1] = LoadTexture(filePath2);

    // Rectangles 
    m_animationRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height }; 
    m_hitboxRect = { 0.0f, 0.0f, (float)m_animationTextures[0].width / 6.0f, (float)m_animationTextures[0].height };

    // frame stuff
    m_currentFrame = 0; // Starting frame
    m_frameCount = frameCount; // Number of frames in the idle animation
    m_runningTime = 0.0f; 
    m_updateTime = 1.0f / 12.0f; 
    m_idle = true; 
    m_direction = 0;
    m_lastDirection = 0;

    // position and speed
    m_positionX = rand() % 900; 
    m_positionY = rand() % 400; 
    m_playerSpeed = 3.0f; //temp, not meant for animatio class
}

Animation::Animation(const char* filePath, const char* filePath2, const char* filePath3, int frameCount){
    
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
    m_idle = true; 
    m_direction = 0;
    m_lastDirection = 0;

    // position and speed
    m_positionX = rand() % 900; 
    m_positionY = rand() % 400; 
    m_playerSpeed = 3.0f; //temp, not meant for animatio class
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
    if(m_direction == 1) { //run left
        m_lastDirection = 1;
        DrawTextureRec(m_animationTextures[2], m_animationRect, {m_positionX, m_positionY}, WHITE);
    }
    else if(m_direction == 2){ //run right
        m_lastDirection = 2;
        DrawTextureRec(m_animationTextures[1], m_animationRect, {m_positionX, m_positionY}, WHITE);
    }
    else{
        DrawTextureRec(m_animationTextures[0], m_animationRect, {m_positionX, m_positionY}, WHITE);
    }
}

void Animation::drawHitbox(){
    DrawRectangleLines(m_positionX, m_positionY, m_hitboxRect.width, m_hitboxRect.height, RED);
}

void Animation::Update(){

    bool wasIdle = m_idle;
    m_idle = true;
    m_direction = 0;

    // temp, not meant for animation class
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){ 
        m_idle = false;
        m_direction = 2; 
        m_positionX += m_playerSpeed;
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        m_idle = false;
        m_direction = 1; 
        m_positionX -= m_playerSpeed;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){ 
        m_idle = false;
        m_direction = m_lastDirection;
        m_positionY -= m_playerSpeed -0.5f;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){ 
        m_idle = false;
        m_direction = m_lastDirection;
        m_positionY += m_playerSpeed - 0.5f;
    }

    // Reset animation if state changed
    if (wasIdle != m_idle) {
        m_currentFrame = 0;
        m_runningTime = 0.0f;
    }

    m_hitboxRect.x = m_positionX; 
    m_hitboxRect.y = m_positionY;
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

Rectangle Animation::getAnimationRect() { 
    return m_hitboxRect; 
}

// setters
void Animation::setPosition(float x, float y){
    m_positionX = x;
    m_positionY = y;
}

// temrporary speed, not meant for animation class
void Animation::setPlayerSpeed(float speed){
    m_playerSpeed = speed;
}
