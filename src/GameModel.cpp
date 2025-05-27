#include "GameModel.h"
#include <cstdlib> // rand
#include <iostream>
#include <map>

GameModel::GameModel() {
  // 初始化場上的物件
  for (int i = 0; i < 5; ++i) {
    auto obj = std::make_unique<RPSGameObject>();
    obj->setPosition({rand() % 10, rand() % 10});
    obj->setDirection(static_cast<Direction>(rand() % 4));
    obj->setType(static_cast<RPSType>(rand() % 3));
    objects.push_back(std::move(obj));
  }
}

void GameModel::update() {
  if (gameOver) return;

  // 移動
  for (auto &obj : objects) {
    obj->update();
  }

  // 碰撞檢查
  for (size_t i = 0; i < objects.size(); ++i) {
    for (size_t j = i + 1; j < objects.size(); ++j) {
      if (objects[i]->intersect(objects[j].get())) {
        objects[i]->onCollision(objects[j].get());
      }
    }
  }

  // 勝負判定
  std::map<RPSType, int> count;
  for (const auto &obj : objects) {
    count[obj->getType()]++;
  }

  int typeCount = 0;
  RPSType lastType;
  for (auto &[type, num] : count) {
    if (num > 0) {
      typeCount++;
      lastType = type;
    }
  }

  if (typeCount == 1) {
    gameOver = true;
    std::string winner;
    switch (lastType) {
      case ROCK: winner = "ROCK"; break;
      case PAPER: winner = "PAPER"; break;
      case SCISSORS: winner = "SCISSORS"; break;
    }
    std::cout << "Game Over! " << winner << " wins!" << std::endl;
  }
}

void GameModel::handleInput(Direction dir) {
  if (controlledIndex < objects.size()) {
    objects[controlledIndex]->setDirection(dir);
  }
}

void GameModel::switchControl() {
  size_t count = objects.size();
  for (size_t i = 1; i < count; ++i) {
    size_t idx = (controlledIndex + i) % count;
    if (objects[idx]->getType() == ROCK) {
      controlledIndex = idx;
      break;
    }
  }
}

const std::vector<std::unique_ptr<RPSGameObject>> &GameModel::getObjects() const {
  return objects;
}

size_t GameModel::getControlledIndex() const {
  return controlledIndex;
}

