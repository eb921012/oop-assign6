// TODO implementation


#include "gameObjectFactory.h"
#include "iconFactory.h"
#include "gameObject.h"

// --- 加入子類別定義 ---
class Rock : public GameObject {
public:
    Rock(int x, int y) {
        _pos = Position{x, y};
        _icon = IconFactory::createRock();
    }
    std::string getType() const override {
        return "RR";
    }
};

class Paper : public GameObject {
public:
    Paper(int x, int y) {
        _pos = Position{x, y};
        _icon = IconFactory::createPaper();
    }
    std::string getType() const override {
        return "PP";
    }
};

class Scissors : public GameObject {
public:
    Scissors(int x, int y) {
        _pos = Position{x, y};
        _icon = IconFactory::createScissors();
    }
    std::string getType() const override {
        return "SS";
    }
};

class Player : public GameObject {
public:
    Player(int x, int y) {
        _pos = Position{x, y};
        _icon = IconFactory::createPlayer();
    }
    std::string getType() const override {
        return "ME";
    }
};

// --- 使用子類別建立物件 ---
GameObject* GameObjectFactory::createRock(int x, int y) {
    return new Rock(x, y);
}

GameObject* GameObjectFactory::createPaper(int x, int y) {
    return new Paper(x, y);
}

GameObject* GameObjectFactory::createScissors(int x, int y) {
    return new Scissors(x, y);
}

GameObject* GameObjectFactory::createPlayer(int x, int y) {
    return new Player(x, y);
}

