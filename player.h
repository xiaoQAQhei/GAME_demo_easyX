#pragma once
#include <graphics.h>
#include <vector>
#include <string>
#include "bullet.h"
#include "util.h"
using namespace std;
// 前向声明，解耦类定义
class rebound;
class bullet;

extern const int GameW;
extern const int GameH;
const int PlayerHP = 20;
extern bool gameover;

// 全局图像资源
extern std::vector<IMAGE> shadow;
extern std::vector<IMAGE> nut_run_left;
extern std::vector<IMAGE> nut_run_right;
extern std::vector<IMAGE> bow_pulling_left;
extern std::vector<IMAGE> bow_pulling_right;
extern std::vector<IMAGE> crossbow_pulling_left;
extern std::vector<IMAGE> crossbow_pulling_right;
extern std::vector<IMAGE> nut_die;
extern std::vector<IMAGE> paimon_die;
extern std::vector<IMAGE> P_winner;
extern IMAGE P_winner_idx;
extern IMAGE iron_door;
extern IMAGE signboard;
extern IMAGE P1;
extern IMAGE P2;
extern IMAGE alert;
extern IMAGE golden_chestplate;
extern IMAGE avatar_paimon;
extern IMAGE avatar_nut;
extern IMAGE normal_arrow;

class player {
public:
    player(double x1, double y1, const int r1, std::wstring s1, int choose1, int player_idx);
    ~player();

    void loadAnimation(std::wstring s);
    void processdeal(ExMessage m);
    void Move();
    void Shot();
    void Rebound(player& enemy);
    void Hurt(player& enemy);
    void Live();
    void Counttimer();
    void Draw();
    void Reset();
    void BulletFilling();

    double x;
    double y;
    const int r = 35;
    double vy = 0;
    double vx = 0;
    int jumpfreq = 2;

    std::vector<bullet> bullet_list;
    rebound* reb = nullptr;

    int choose;
    const int HP_max = 20;
    int HP = HP_max;
    const int location_r;
    bool isAboveGround = false;
    int shadow_idx = 0;
    int shadow_x = 0;
    int shadow_y = 2147483647; // INT_MAX 代替值
    int weaponState_idx = 0;
    bool isDeath = false;

    const int maxBulletAmount = 5;
    int bullet_amount = maxBulletAmount;
    const int bulletFillingdelay = 100;
    int bulletFillingtimer = 0;

private:
    double temp_x, temp_y;
    int player_idx;
    const double speed = 0.6;
    const double maxSpeed = 8.0;
    const double PlusRepelSpeed = 40.0;
    const double repelamount = 25.0;
    const double jumpspeed = 25.0;
    const double friction = 0.4;

    int goldtimer = 0;
    std::vector<IMAGE> player_left;
    std::vector<IMAGE> player_right;
    IMAGE player_pic_idx;

    int current_idx = 0;
    int counter = 0;

    bool player_p = true;
    bool singlejump = true;
    bool jumpwalk = false;
    bool downwalk = false;
    bool rightwalk = false;
    bool leftwalk = false;

    int bullet_shot_timer = 0;
    bool single_b_s = false;
    bool single_reb = false;

    const int enter_player_cursor_delay = 100;
    int enter_player_cursor_timer = enter_player_cursor_delay;

    bool WinnerAnimation = false;
    const int overgameAnimation_delay = 100;
    int overgameAnimation_timer = overgameAnimation_delay;

    const int plusBulletDamage = 5;
    const int bulletDamage = 1;

    int HP_x;
    int bulletfilling_x;
};
