#pragma once
#include "animation.h"

class Enemy : public Animation {

    private:
        // enemy variables
        float m_enemySpeed;

        // health stuff
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
        void takeDamage(int damage);
        void animateSprite();
        void drawHealthBar();
        void drawHurtFrame();
        void updateSprite();

        //getters and setters
        int getHealth();
        void setHealth(int health);
        Vector2 getPosition();
        void setEnemySpeed(float speed);

};
