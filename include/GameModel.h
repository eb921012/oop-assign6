#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "RPSGameObject.h"
#include <memory>
#include <vector>
#include "GameModel.h"

class GameModel {
public:
  GameModel();

  void update(); // 移動與碰撞邏輯
  void handleInput(Direction dir); // 控制玩家物件的方向
  void switchControl(); // 切換控制的 ROCK
  const std::vector<std::unique_ptr<RPSGameObject>> &getObjects() const;
  size_t getControlledIndex() const;

private:
  std::vector<std::unique_ptr<RPSGameObject>> objects;
  size_t controlledIndex = 0;
  GameModel _model;
};

#endif

