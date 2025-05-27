#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <termios.h>
#include <vector>
#include <string>
#include "view.h"
#include "gameObject.h"

class Controller {

public:
    Controller(View&);
    void run();
    void printStatus() const;

    // 撞擊偵測 + 剪刀石頭布勝負判斷
    GameObject* getObjectAt(Position pos);
    bool canDefeat(const std::string& me, const std::string& other);

private:
    void handleInput(int);
    void update();
    void switchToNextRR();  // 新增：切換到下一個RR
    bool hasRR();           // 新增：檢查場上是否還有RR
    void switchToFirstRR(); // 新增：切換控制到第一個 RR
    void checkPlayerType(); // 新增：檢查玩家是否仍為 RR，否則自動切換或結束

    // Model
    std::vector<GameObject*> _objs;
    GameObject* _player;
    GameObject* _originalPlayer; // 新增：記錄最初的 player
    size_t _currentIndex; // 記錄目前控制的是 _objs 中的哪個

    // View
    View& _view;
};

static struct termios old_termios, new_termios;
void reset_terminal();
void configure_terminal();
int read_input();

#endif