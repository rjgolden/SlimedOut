#pragma once
#include "animation.h"

class Enemy : public Animation {

    private:
        float m_enemySpeed;
        float m_health;
        Rectangle m_healthBarRect; 
        Texture2D m_enemyHurt;
        bool m_hurtFrameActive = false;
        
    public:
        Enemy(const char* filePath, int frameCount);
        Enemy(const char* filePath, const char* filePath2, const char* filePath3, int frameCount);
        Enemy();
        ~Enemy();

        void chasePlayer(float playerX, float playerY);
 
        int getHealth();
        void setEnemySpeed(float speed);
        void setHealth(int health);
        void takeDamage(float damage);

        void animateSprite();
        void drawHealthBar();
        void drawHurtFrame();
        void updateSprite();

};
