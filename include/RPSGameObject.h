#ifndef RPS_H
#define RPS_H

#include "collider.h"
#include "gameObject.h"
#include "unit.h"

enum RPSType { ROCK, PAPER, SCISSORS };

class RPSGameObject : public GameObject, public ICollider {

public:
  void update() override;
  void onCollision(ICollider *other) override;
  bool intersect(ICollider *other) override;

  RPSType getType() const;
  void setType(RPSType type);
  void setDirection(Direction);

private:
  RPSType _type;
  Direction _dir;
};
#endif
