#pragma once
#include <raylib.h>
#include <iostream>

class Animation {

    protected:
        Texture2D m_animationTextures[5]; // Textures for the animation (idle and moving left and right for now)
        Rectangle m_animationRect; // Source rectangle for the animation
        Rectangle m_hitboxRect; // Hitbox for the animation

        unsigned m_currentFrame; // Current frame of the animation
        unsigned m_frameCount; 
        float m_runningTime;  // Time accumulator for the animation
        float m_updateTime; // Time between frame updates
        float m_positionX; 
        float m_positionY; 

    public: 
        Animation(const char* filePath, int frameCount, float positionX, float positionY);
        Animation();
        ~Animation();
        
        void animateSprite();
        void drawSprite();
        void drawHitbox();
        void updateSprite();

        // getters
        float getPositionX();
        float getPositionY(); 
        float getWidth();
        float getHeight();
        Rectangle getHitboxRect();

        // setters
        void setPosition(float x, float y);
};