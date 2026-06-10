#include"util.h"
#include"player.h"

// 碰撞检测逻辑：根据包围盒 bx, by, bw, bh 检测 player p 的位置，并修正其位置。
void CollisionBoxDetction(player& p, const double& bx, const double& by, const double& bw, const double& bh) {    
    // 左右边界判定：只有当玩家处于方块横向跨度内时才触发逻辑
    if (p.x <= bx + bw && p.x >= bx) {
        p.isAboveGround = true;

        // 玩家下落触顶逻辑
        if (p.y - p.r + p.vy <= by + bh && p.y > by) {
            p.vy = 0;
            p.y = by + bh + p.r;
        }
        // 玩家落地判定
        else if (p.y + p.r + p.vy >= by && p.y < by) {
            p.vy = 0;
            p.y = by - p.r;
            p.jumpfreq = 2; // 恢复两段跳
        }
    }
    
    // 横向碰撞判定：处理墙壁逻辑
    if (p.y < by + bh && p.y > by && p.x + p.r > bx && p.x - p.r < bx + bw) {
        if (p.x - p.r < bx + bw && p.x > bx + bw) {
            p.x = bx + bw + (double)p.r;
            p.jumpfreq = 2;
        }
        else if (p.x + (double)p.r >= bx && p.x < bx) {
            p.x = bx - (double)p.r;
            p.jumpfreq = 2;
        }
        p.vx = -p.vx; // 弹回逻辑
    }

    // 阴影渲染逻辑
    if (p.isAboveGround) {
        if (p.jumpfreq == 2) {
            p.shadow_idx = 0;
            p.shadow_x = (int)p.x - 24;
            p.shadow_y = (int)(abs(p.y-by) < abs(p.y-p.shadow_y) && p.y < by && p.x <= bx + bw && p.x >= bx ? by : (double)p.shadow_y);
        }
        else {
            p.shadow_idx = 1;
            p.shadow_x = (int)p.x - 16;
            p.shadow_y = (int)(abs(p.y - by) < abs(p.y - p.shadow_y) && p.y < by && p.x <= bx + bw && p.x >= bx ? by : (double)p.shadow_y);
        }
    }
}
