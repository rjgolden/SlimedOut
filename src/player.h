#pragma once
#include "animation.h"

class Player: public Animation {

    private:
        float m_playerSpeed; 
        bool m_idle; 
        unsigned m_direction; 
        unsigned m_lastDirection;
        Texture2D* m_currentTexture;  // Pointer to current texture
        int m_currentState;// Track current state

        Rectangle m_attackRect;
        Sound m_swordSlashSound;

    public:
        Player(const char* filePath, const char* filePath2, const char* filePath3, int frameCount);
        Player();
        ~Player();

        void animateSprite();
        void drawSprite();
        void drawAttackHitbox();
        void updateSprite();
        void setPlayerSpeed(float speed);
        void setState(int newState);
        Rectangle getAttackRect();

};