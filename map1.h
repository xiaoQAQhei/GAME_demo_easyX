#pragma once

#include <graphics.h>
#include <string>
#include <vector>
#include"map.h"
#include"util.h"

extern vector<IMAGE> shadow;
extern IMAGE GameMap;
extern const int GameW;
extern const int GameH;

class Map1 : public Map {
public:
    Map1() = default;
    ~Map1() = default;

    void on_enter() override {
        mciSendString(L"play bgm1 repeat from 0", NULL, 0, NULL);
    }

    void on_exit() override {
        mciSendString(L"stop bgm1 ", NULL, 0, NULL);
    }

    void Render() override {
        // 背景已经在 GameScene 中绘制，这里可以添加地图特有装饰
    }

    void on_update(player& p) override {
        // 应用重力
        p.vy += g;

        // 地面碰撞检测 (简单地平线)
        if (p.y + p.r + p.vy >= bottom) {
            p.vy = 0;
            p.y = bottom - p.r;
            p.jumpfreq = 2; // 恢复跳跃次数
            p.isAboveGround = true;
            
            // 更新阴影坐标
            p.shadow_idx = (p.jumpfreq == 2) ? 0 : 1;
            p.shadow_x = (int)p.x - (p.shadow_idx == 0 ? 24 : 16);
            p.shadow_y = bottom;
        } else {
            p.isAboveGround = false;
        }
    }

private:
    const double g = 0.8; // 平滑重力
    const int bottom = 900 / 2 + 15; // 地面高度
};
