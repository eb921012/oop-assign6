#include "RPSGameObject.h"
#include "unit.h"
#include "iconFactory.h"
#include <cstdlib>
#include <iostream>

// === 判斷勝負規則 ===
bool winsAgainst(RPSType self, RPSType other) {
    return (self == ROCK && other == SCISSORS) ||
           (self == SCISSORS && other == PAPER) ||
           (self == PAPER && other == ROCK);
}

// === 簡單 getter/setter ===
RPSType RPSGameObject::getType() const {
    return _type;
}

void RPSGameObject::setType(RPSType type) {
    _type = type;

    // 自動更新對應圖示
    switch (type) {
        case ROCK:
            _icon = IconFactory::createRock();
            break;
        case PAPER:
            _icon = IconFactory::createPaper();
            break;
        case SCISSORS:
            _icon = IconFactory::createScissors();
            break;
    }
}

void RPSGameObject::setDirection(Direction dir) {
    _dir = dir;
}

// === 移動物件位置 ===
void RPSGameObject::update() {
    switch (_dir) {
        case UP:
            _pos.y -= 1;
            break;
        case DOWN:
            _pos.y += 1;
            break;
        case LEFT:
            _pos.x -= 1;
            break;
        case RIGHT:
            _pos.x += 1;
            break;
    }
}

// === 判斷是否碰撞（位置相同） ===
bool RPSGameObject::intersect(ICollider *other) {
    auto *otherObj = dynamic_cast<RPSGameObject *>(other);
    if (!otherObj) return false;
    return this->_pos == otherObj->_pos;
}

// === 碰撞後的同化邏輯 ===
void RPSGameObject::onCollision(ICollider *other) {
    auto *otherObj = dynamic_cast<RPSGameObject *>(other);
    if (!otherObj) return;

    RPSType myType = this->getType();
    RPSType otherType = otherObj->getType();

    if (winsAgainst(myType, otherType)) {
        otherObj->setType(myType);  // 同化對方
    } else if (winsAgainst(otherType, myType)) {
        this->setType(otherType);   // 被對方同化
    }
    // 平手什麼都不做
}

