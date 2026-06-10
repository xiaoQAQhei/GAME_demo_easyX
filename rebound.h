#pragma once
#include "bullet.h"

// 前向声明，避免循环引用
class player;

class rebound {
public:
    void check(player& p, bullet& b);
    void parryCooldown(int &choose);
    void bullet_rebound(bullet& b);
    void Reset();

    bool isParrying = false;
    bool canParrying = false;
    const int parryDuration_max = 20;
    int parryDuration = parryDuration_max;
    const int t = 3;
    int timer = t;

private:
};
