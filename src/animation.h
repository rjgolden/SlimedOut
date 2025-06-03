#pragma once
#include <raylib.h>
#include <iostream>

class Animation {

    private:
        Texture2D m_animationTextures[3]; // Textures for the animation (idle and moving left and right for now)
        Rectangle m_animationRect; // Source rectangle for the animation
        Rectangle m_hitboxRect; // Hitbox for the animation

        unsigned m_currentFrame; // Current frame of the animation
        unsigned m_frameCount; 
        float m_runningTime;  // Time accumulator for the animation
        float m_updateTime; // Time between frame updates
        bool m_idle; 

        unsigned m_direction; 
        unsigned m_lastDirection;
        float m_positionX; 
        float m_positionY; 
        float m_playerSpeed; // temporary speed variable, not meant for animation class

    public: 
        Animation(const char* filePath, const char* filePath2, int frameCount);
        Animation(const char* filePath, const char* filePath2, const char* filePath3, int frameCount);
        Animation();
        ~Animation();
        
        void animateSprite();
        void drawSprite();
        void drawHitbox();
        void Update();

        // getters
        float getPositionX();
        float getPositionY(); 
        float getWidth();
        float getHeight();
        Rectangle getAnimationRect();

        // setters
        void setPosition(float x, float y);
        void setPlayerSpeed(float speed); //temporary speed variable, not meant for animation class
};