#pragma once

#include <graphics.h>
#include <string>
#include <vector>
#include "map.h"
#include "util.h"

extern vector<IMAGE> shadow;
extern IMAGE GameMap;

class Map2 : public Map {
public:
    Map2() = default;
    ~Map2() = default;

    void on_enter() override {
        mciSendString(L"play bgm2 repeat from 0", NULL, 0, NULL);
    }

    void on_exit() override {
        mciSendString(L"stop bgm2 ", NULL, 0, NULL);
    }

    void Render() override {
        // 背景已经在 GameScene 中绘制
    }

    void on_update(player& p) override {
        p.vy += map2_g;

        // 碰撞检测逻辑：根据不同的方块区域进行检测
        CollisionBoxDetction(p, (double)bottom1_x, (double)bottom1_y, (double)bottom1_w, (double)bottom1_h);
        CollisionBoxDetction(p, (double)bottom2_left_x, (double)bottom2_left_y, (double)bottom2_left_w, (double)bottom2_left_h);
        CollisionBoxDetction(p, (double)bottom2_right_x, (double)bottom2_right_y, (double)bottom2_right_w, (double)bottom2_right_h);
        CollisionBoxDetction(p, (double)bottom3_x, (double)bottom3_y, (double)bottom3_w, (double)bottom3_h);
    }

private:
    const double map2_g = 0.8; // 平滑重力

    // 地图方块坐标常量
    const int bottom1_x = 455;
    const int bottom1_y = 695;
    const int bottom1_w = 690;
    const int bottom1_h = 63;

    const int bottom2_left_x = 274;
    const int bottom2_left_y = 550;
    const int bottom2_left_w = 203;
    const int bottom2_left_h = 63;

    const int bottom2_right_x = 1123;
    const int bottom2_right_y = 550;
    const int bottom2_right_w = 203;
    const int bottom2_right_h = 63;

    const int bottom3_x = 610;
    const int bottom3_y = 430;
    const int bottom3_w = 380;
    const int bottom3_h = 60;
};
