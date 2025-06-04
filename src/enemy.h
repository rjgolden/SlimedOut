#pragma once
#include "player.h"

class Enemy : public Animation {

    private:
        float m_enemySpeed;
        
    public:
        Enemy(const char* filePath, int frameCount);
        Enemy(const char* filePath, const char* filePath2, const char* filePath3, int frameCount);
        Enemy();
        ~Enemy();

        void chasePlayer(float playerX, float playerY);
        void setEnemySpeed(float speed);

};
