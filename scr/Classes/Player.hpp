#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Util.hpp"
#include "../Libs/nTLoader.h"
class Player {
public:
    Player();
    Player(const Player& orig);
    virtual ~Player();
    static void jump();
    static void applyGravity();
    static void draw();
    static void spawn(nTPoint pos);
    static void syncWithCamera();
    static void moveFoward(float dir);
    static void moveSide(int dir);
    static bool noCollisionWithMap(nTPoint posC);
    static void checkGG();
    static float life;
    static bool livingWell;
    static nTPoint pos;
    static nTPoint size;
    static bool firstPerson;
    static bool dontGiveAShitForCollision;
    static nTMesh jugg;
private:
    static bool canJump;
    static float vSpeed;
};

#endif /* PLAYER_HPP */

