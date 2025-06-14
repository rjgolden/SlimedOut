#pragma once
#include "animation.h"

class Player: public Animation {

    private:
        //player variables
        float m_playerSpeed; 
        int m_playerHealth;
        bool m_hurtFrameActive; // Need to implement this
    
        // frame variables
        bool m_idle; 
        unsigned m_direction; 
        unsigned m_lastDirection;
        Texture2D* m_currentTexture;  // Pointer to current texture
        int m_currentState;// Track current state of textures 

        // extra variables
        Rectangle m_healthBarRect; 
        Rectangle m_attackRect;
        Sound m_swordSlashSound;

    public:
        Player(const char* filePath, const char* filePath2, const char* filePath3, int frameCount);
        Player(const char* filePath, const char* filePath2, const char* filePath3, const char* filePath4, const char* filePath5, int frameCount);
        Player();
        ~Player();

        void drawSprite();
        void drawAttackHitbox();
        void setState(int newState);
        void animateSprite();
        void updateSprite();
        void drawHealthBar();
        void takeDamage(int damage);
        bool isAnimationComplete();

        //getters and setters
        Rectangle getAttackRect();
        int getHealth();
        void setPlayerSpeed(float speed);
        void setHealth(int health);
};