// TODO 
// Finish the declaration of GameObjectFactory that
// 1. offers some methods to create a GameObject.
// 2. encapsulates the constructor from the caller. 

#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "gameObject.h"

class GameObjectFactory {
public:
    static GameObject* createRock(int x, int y);     // 石頭
    static GameObject* createPaper(int x, int y);    // 布
    static GameObject* createScissors(int x, int y); // 剪刀
    static GameObject* createPlayer(int x, int y);   // 玩家（可能可選）
};

#endif
