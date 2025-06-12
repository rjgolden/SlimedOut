#pragma once
#include "animation.h"

class Player: public Animation {

    private:
        float m_playerSpeed; 
        int m_playerHealth;
        bool m_hurtFrameActive; // Need to implement this
        Rectangle m_healthBarRect; 
        bool m_idle; 
        unsigned m_direction; 
        unsigned m_lastDirection;
        Texture2D* m_currentTexture;  // Pointer to current texture
        int m_currentState;// Track current state of textures 

        Rectangle m_attackRect;
        Sound m_swordSlashSound;

    public:
        Player(const char* filePath, const char* filePath2, const char* filePath3, int frameCount);
        Player();
        ~Player();

        void animateSprite();
        void drawSprite();
        void drawAttackHitbox();
        void drawHealthBar();
        void takeDamage(int damage);
        void setHealth(int health);
        int getHealth();
        void updateSprite();
        void setPlayerSpeed(float speed);
        void setState(int newState);
        Rectangle getAttackRect();

};