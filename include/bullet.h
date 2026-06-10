#pragma once
#include <graphics.h>
#include <vector>

// 前向声明，避免包含 util.h 导致的循环依赖
class IMAGE;

extern std::vector<IMAGE> arrow;
extern std::vector<IMAGE> spectral_arrow;
extern std::vector<IMAGE> firework;

const int plus_time = 15;
extern const int GameW;
extern const int GameH;

class bullet {
public:
    // 构造函数参数改为 double
    bullet(double x, double y, bool dir, int choose, int b_s_time);

    void move();
    void check();
    void draw();
    bool getdir();

    bool isPlusShot = false;
    double b_x;
    double b_y;
    int b_r = 20;
    int plus_b_r = 50;
    double bullet_speed = 30.0;
    bool bullet_dir;

private:
    int bullet_shot_time;
    int choose;
};

// 接口函数声明
void shotbullet(double x, double y, bool dir, int choose, int bst, std::vector<bullet>& bullet_list);
void movebullet(std::vector<bullet>& bullet_list);
void drawbullets(std::vector<bullet>& bullet_list);
