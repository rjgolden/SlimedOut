#include "player.h"

// Default and Destructor
Player::Player(){
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
    m_direction = 0;
    m_lastDirection = 0;

    // position and speed
    m_positionX = rand() % 900;
    m_positionY = rand() % 400;
    m_playerSpeed = 3.0f; //temp, not meant for animatio class
}
Player::~Player() {
    UnloadTexture(m_animationTextures[0]);
    UnloadTexture(m_animationTextures[1]);
    UnloadTexture(m_animationTextures[2]);
}

// meat and potatoes
Player::Player(const char* filePath, const char* filePath2, const char* filePath3, int frameCount) {
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
    m_idle = true; 
    m_direction = 0;
    m_lastDirection = 0;
    m_playerSpeed = 3.0f; 
}

void Player::drawSprite(){
    DrawTextureRec(*m_currentTexture, m_animationRect, {m_positionX, m_positionY}, WHITE);
}

void Player::setState(int newState){
    if (newState != m_currentState) {  // Only change when different
        std::cout << "setState called\n";
        m_currentState = newState;
        m_lastDirection = newState;

        // Set texture once when state changes
        switch(newState) {
        case 1: m_currentTexture = &m_animationTextures[2]; break;  // left
        case 2: m_currentTexture = &m_animationTextures[1]; break;  // right
        default: m_currentTexture = &m_animationTextures[0]; break; // idle
        }
    }
}

void Player::updateSprite() {

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
    setState(m_direction);  // Expensive work only happens when needed
    animateSprite();
    drawSprite();
    //drawHitbox();

}

void Player::setPlayerSpeed(float speed) {
    m_playerSpeed = speed;
}