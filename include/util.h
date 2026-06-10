#pragma once

#include <graphics.h>
#include <cmath>
#include <string>
#include <iostream>

// 前向声明，让 util 不用包含 player.h
class player;

inline void flip_image(IMAGE* src, IMAGE* dst) {
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dst, w, h);
    DWORD* src_buffer = GetImageBuffer(src);
    DWORD* dst_buffer = GetImageBuffer(dst);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int idx_src = y * w + x;
            int idx_dst = y * w + (w - x - 1);
            dst_buffer[idx_dst] = src_buffer[idx_src];
        }
    }
}

#pragma comment(lib,"MSIMG32.LIB")
inline void Alpha_putimage(int x, int y, IMAGE& img, unsigned char alpha = 255) {
    int w = img.getwidth();
    int h = img.getheight();
    AlphaBlend(GetImageHDC(), x, y, w, h, GetImageHDC(&img), 0, 0, w, h, { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA });
}

// distance 函数现在处理 double，提高碰撞判定平滑度
inline double distance(double x1, double y1, double x2, double y2) {
    double d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    return d;
}

// 碰撞判定函数，支持 double 类型的地图边界
void CollisionBoxDetction(player& p, const double& bx, const double& by, const double& bw, const double& bh);
