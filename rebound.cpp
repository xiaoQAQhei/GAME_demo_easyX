#include "rebound.h"
#include "player.h"
#include "bullet.h"
#include "util.h"
#include <iostream>

void rebound::check(player& p, bullet& b) {
    if (isParrying) {
        // 增加 10 像素偏移以优化判定
        if (distance(b.b_x + 10.0, b.b_y, p.x, p.y) < (double)b.plus_b_r + (double)p.r) {
             canParrying = true; // 反弹成功
             mciSendString(L"play rebound from 0", NULL, 0, NULL);
        }
    }
}

void rebound::parryCooldown(int& choose) {
    if (timer >= t && !isParrying) {
        isParrying = true;
        if (choose == 1) {
            mciSendString(L"play signboard from 0", NULL, 0, NULL);
        } else if (choose == 2) {
            mciSendString(L"play iron_door from 0", NULL, 0, NULL);
        }
        timer = 0;
    }
}

void rebound::bullet_rebound(bullet& b) {
    if (canParrying) {
        // 成功反弹后大幅增加子弹速度
        b.bullet_speed += 20.0;
        b.bullet_dir = !b.bullet_dir; // 反转方向
    }
}

void rebound::Reset() {
    isParrying = false;
    canParrying = false;
    timer = t;
}
