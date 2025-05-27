#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <algorithm>
#include "environment.h"
#include "controller.h"
#include "gameObjectFactory.h"

// 地圖大小
const int W = GAME_WINDOW_WIDTH;
const int H = GAME_WINDOW_HEIGHT;

Controller::Controller(View& view) : _view(view) {
    // 初始化玩家位置
    _player = GameObjectFactory::createRock(5, 5);
    _objs.push_back(_player);
    _currentIndex = 0;  // ★ 初始控制 _objs[0]，也就是玩家

    // 加入一些測試物件（可以改成剪刀、石頭、布）
    _objs.push_back(GameObjectFactory::createRock(7, 5));
    _objs.push_back(GameObjectFactory::createRock(2, 13));
    _objs.push_back(GameObjectFactory::createRock(6, 5));
    _objs.push_back(GameObjectFactory::createPaper(5, 3));
    _objs.push_back(GameObjectFactory::createPaper(12, 6));
    _objs.push_back(GameObjectFactory::createPaper(16, 2));
    _objs.push_back(GameObjectFactory::createScissors(6, 15));
    _objs.push_back(GameObjectFactory::createScissors(1, 5));
    _objs.push_back(GameObjectFactory::createScissors(3, 8));
}

void Controller::run() {
    // initial setup
    std::cin.tie(0);
    std::ios::sync_with_stdio(0);
    configure_terminal();

    // init state
    int input = -1;
    clock_t start, end;

    this->update();

    // Main loop
    while (true) {
        start = clock();
        // game loop goes here
        input = read_input();

        // ESC to exit program
        if(input==27) break;

        this->handleInput(input);

        // 更新所有 NPC 的隨機移動與同化
        this->update();

        _view.resetLatest();
        for(GameObject* obj : _objs) {
            obj->update();
            _view.updateGameObject(obj);
        }

        _view.render();

        end = clock();

        // frame rate normalization
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (time_taken > SPF) continue;
        int frameDelay = int((SPF - time_taken) * 1000); // 0.1 seconds
        if(frameDelay > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay)); // frame delay
    }
}

GameObject* Controller::getObjectAt(Position pos) {
    for (GameObject* obj : _objs) {
        if (obj->getPosition() == pos) {
            return obj;
        }
    }
    return nullptr;
}

bool Controller::canDefeat(const std::string& me, const std::string& other) {
    if (me == "RR" && other == "SS") return true;
    if (me == "PP" && other == "RR") return true;
    if (me == "SS" && other == "PP") return true;
    return false;
}

//新增功能，顯示場地上的數量
void Controller::printStatus() const {
    int cntR = 0, cntP = 0, cntS = 0;
    for (auto* obj : _objs) {
        if (obj->getType() == "RR") cntR++;
        else if (obj->getType() == "PP") cntP++;
        else if (obj->getType() == "SS") cntS++;
    }
    std::cout << "[Status] RR: " << cntR
              << ", PP: " << cntP
              << ", SS: " << cntS << std::endl;
}

    void Controller::switchToNextRR() {
    size_t n = _objs.size();
    for (size_t i = 0; i < n; ++i) {
        size_t idx = (_currentIndex + i) % n;
        if (_objs[idx]->getType() == "RR") {
            _currentIndex = idx;
            _player = _objs[_currentIndex];
            // std::cout << "[DEBUG] Auto-switched control to Obj[" << idx
            //           << "] type=" << _player->getType() << "\n";
            return;
        }
    }

    // 沒有可控的 RR，遊戲結束
    _view.resetLatest();
    for(GameObject* obj : _objs) {
        obj->update();
        _view.updateGameObject(obj);
    }
    _view.render();
    printStatus();
    std::cout << "You lose!\n";
    exit(0);
}


void Controller::handleInput(int keyInput) {

    // If there is no input, do nothing.
    if (keyInput == -1) return;

    //     //  DEBUG: 打印按下的鍵
    // std::cout << "[DEBUG] Key pressed: " << char(keyInput) << "\n";

    //  切換控制的 RR（按 C 鍵即可切換至下一個 RR）
    if (keyInput=='c' || keyInput=='C') {
        std::cout << "[DEBUG] Enter switch-RR logic\n";
        size_t n = _objs.size();
        for (size_t i = 1; i < n; ++i) {
            size_t idx = (_currentIndex + i) % n;
            if (_objs[idx]->getType() == "RR") {
                _currentIndex = idx;
                _player = _objs[_currentIndex];
                // std::cout << "[DEBUG] Switched control to Obj[" << idx
                //           << "] type=" << _player->getType() << "\n";
                break;
            }
        }
        return; // 切換完畢就返回，不做其他動作
    }

    // // —— DEBUG 區塊開始 ——
    // std::cout << "[DEBUG] Player at ("
    //           << _player->getPosition().x() << ","
    //           << _player->getPosition().y() << ")\n";
    // for (GameObject* obj : _objs) {
    //     std::cout << "[DEBUG] Obj " << obj->getType()
    //               << " at (" << obj->getPosition().x()
    //               << "," << obj->getPosition().y() << ")\n";
    // }
    // // —— DEBUG 區塊結束 ——

    // 1. 計算 newPos
    Position oldPos = _player->getPosition();
    Position newPos = oldPos;
    switch (keyInput) {
        case 'w': case 'W': newPos.y() -= 1; break;
        case 's': case 'S': newPos.y() += 1; break;
        case 'a': case 'A': newPos.x() -= 1; break;
        case 'd': case 'D': newPos.x() += 1; break;
        default: return;
    }

    // 2. 邊界檢查
    if (newPos.x() < 0 || newPos.x() >= W ||
        newPos.y() < 0 || newPos.y() >= H) {
        return;
    }

    // 移動玩家
    _player->setPosition(newPos);

    // // —— DEBUG newPos ——
    // std::cout << "[DEBUG] Moved to ("
    //           << newPos.x() << "," << newPos.y() << ")\n";
    // // —— DEBUG 區塊結束 ——

    // 3. 同化範圍：四個方向
    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    std::string me = _player->getType();

        for (auto &d : dirs) {
        Position np = newPos;
        np.x() += d[0];
        np.y() += d[1];
        if (np.x() < 0 || np.x() >= W ||
            np.y() < 0 || np.y() >= H) continue;

        GameObject* target = getObjectAt(np);
        if (!target) continue;

        std::string ot = target->getType();
        // std::cout << "[DEBUG] Adjacent " << ot
        //           << " at (" << np.x() << "," << np.y() << ")\n";

        if (canDefeat(me, ot)) {
            GameObject* newObj = nullptr;
            if (me == "RR") newObj = GameObjectFactory::createRock(np.x(), np.y());
            if (me == "PP") newObj = GameObjectFactory::createPaper(np.x(), np.y());
            if (me == "SS") newObj = GameObjectFactory::createScissors(np.x(), np.y());
            auto it = std::find(_objs.begin(), _objs.end(), target);
            if (it != _objs.end() && newObj) *it = newObj;
        } else if (canDefeat(ot, me)) {
    GameObject* newPlayer = nullptr;
    if (ot == "RR") newPlayer = GameObjectFactory::createRock(oldPos.x(), oldPos.y());
    if (ot == "PP") newPlayer = GameObjectFactory::createPaper(oldPos.x(), oldPos.y());
    if (ot == "SS") newPlayer = GameObjectFactory::createScissors(oldPos.x(), oldPos.y());

    for (size_t i = 0; i < _objs.size(); ++i) {
        if (_objs[i] == _player) {
            _objs[i] = newPlayer;
            _currentIndex = i;
            break;
        }
    }

    _player = newPlayer;

    // 玩家變成非 RR，自動切換到其他 RR（如果有）
    if (_player->getType() != "RR") {
        // std::cout << "[DEBUG] Player is no longer RR\n";
        switchToNextRR();
    }
}

    }

    // 勝負判定
    int cntR = 0, cntP = 0, cntS = 0;
    for (auto *obj : _objs) {
        if (obj->getType() == "RR") cntR++;
        else if (obj->getType() == "PP") cntP++;
        else if (obj->getType() == "SS") cntS++;
    }

    // 印出即時統計狀態
    printStatus();

    if (cntR == int(_objs.size())) {
            _view.resetLatest();
    for(GameObject* obj : _objs) {
        obj->update();
        _view.updateGameObject(obj);
    }
    _view.render();
        printStatus();
        std::cout << "You win!\n";
        exit(0);
    } else if (cntR == 0) {
            _view.resetLatest();
    for(GameObject* obj : _objs) {
        obj->update();
        _view.updateGameObject(obj);
    }
    _view.render();
        printStatus();
        std::cout << "You lose!\n";
        exit(0);
    }
}

void Controller::update() {
    // 隨機移動所有 NPC（包含玩家以外） + RPS 同化
    std::vector<std::pair<GameObject*,GameObject*>> toAssimilate;

    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (GameObject* obj : _objs) {
        if (obj == _player) continue;
        Position oldPos = obj->getPosition();
        Position newPos = oldPos;
        // 隨機方向移動
        int dir = rand() % 4;
        newPos.x() += dirs[dir][0];
        newPos.y() += dirs[dir][1];
        // 邊界檢查
        if (newPos.x() < 0 || newPos.x() >= W ||
            newPos.y() < 0 || newPos.y() >= H) {
            continue;
        }
        // 設定暫時位置
        obj->setPosition(newPos);
        // 同化：檢查周圍四格
        std::string at = obj->getType();
        for (auto &d : dirs) {
            Position np = newPos;
            np.x() += d[0];
            np.y() += d[1];
            if (np.x() < 0 || np.x() >= W || np.y() < 0 || np.y() >= H)
                continue;
            GameObject* target = getObjectAt(np);
            if (!target || target == _player) continue;

            std::string ot = target->getType();
            if (canDefeat(at, ot)) {
                toAssimilate.emplace_back(obj, target);
            } else if (target == _player && canDefeat(ot, at)) {
                // NPC 吃掉玩家
                exit(0);
            }
        }
    }

    // 執行同化
    for (auto &pr : toAssimilate) {
        GameObject* attacker = pr.first;
        GameObject* target   = pr.second;
        Position p = target->getPosition();
        std::string at = attacker->getType();
        GameObject* newObj = nullptr;
        if (at == "RR") newObj = GameObjectFactory::createRock(p.x(), p.y());
        if (at == "PP") newObj = GameObjectFactory::createPaper(p.x(), p.y());
        if (at == "SS") newObj = GameObjectFactory::createScissors(p.x(), p.y());
        auto it = std::find(_objs.begin(), _objs.end(), target);
        if (it != _objs.end() && newObj) *it = newObj;
    }
}

void reset_terminal() {
    printf("\e[m"); // reset color changes
    printf("\e[?25h"); // show cursor
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

// terminal initial configuration setup
void configure_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios; // save it to be able to reset on exit

    new_termios.c_lflag &= ~(ICANON | ECHO); // turn off echo + non-canonical mode
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    printf("\e[?25l"); // hide cursor
    std::atexit(reset_terminal);
}

int read_input() {
    fflush(stdout);
    char buf[4096]; // maximum input buffer
    int n        = read(STDIN_FILENO, buf, sizeof(buf));
    return n > 0 ? buf[n - 1] : -1;
} 