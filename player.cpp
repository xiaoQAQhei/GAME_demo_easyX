#include "player.h"
#include "rebound.h"
#include <string>
#include <cmath>

player::player(double x1, double y1, int r1, std::wstring s1, int choose1, int player_idx)
	: player_left(6), player_right(6), player_pic_idx(player_left[0]),
	x(x1), y(y1), location_r(r1), choose(choose1), temp_x(x1), temp_y(y1), player_idx(player_idx) {
	loadAnimation(s1);
	reb = new rebound();
}

player::~player() {
	delete reb;
	reb = nullptr;
}

void player::loadAnimation(std::wstring s) {
	for (int i = 0; i < player_idx; i++) {
		std::wstring path_left = L"img/" + s + L"_left_" + std::to_wstring(i) + L".png";
		std::wstring path_right = L"img/" + s + L"_right_" + std::to_wstring(i) + L".png";
		loadimage(&player_left[i], path_left.c_str());
		loadimage(&player_right[i], path_right.c_str());
	}
}

void player::processdeal(ExMessage m) {
	if (choose == 1) {
		if (m.message == WM_KEYDOWN) {
			switch (m.vkcode) {
			case'A':leftwalk = true;break;
			case'D':rightwalk = true;break;
			case'W':jumpwalk = true;break;
			case'S':downwalk = true;break;
			case'G':bullet_shot_timer++;break;
			case'H':if (!reb->timer)reb->timer = 1;reb->parryCooldown(choose);break;
			}
		}
		if (m.message == WM_KEYUP) {
			switch (m.vkcode) {
			case'A':leftwalk = false;break;
			case'D':rightwalk = false;break;
			case'W':jumpwalk = false;singlejump = true;break;
			case'S':downwalk = false;break;
			case'G':single_b_s = true;break;
			}
		}
	}
	else if (choose == 2) {
		if (m.message == WM_KEYDOWN) {
			switch (m.vkcode) {
			case VK_LEFT:leftwalk = true;break;
			case VK_RIGHT:rightwalk = true;break;
			case VK_UP:jumpwalk = true;break;
			case VK_DOWN:downwalk = true;break;
			case VK_NUMPAD1:bullet_shot_timer++;break;
			case VK_NUMPAD2:if (!reb->timer)reb->timer = 1;reb->parryCooldown(choose);break;
			}
		}
		if (m.message == WM_KEYUP) {
			switch (m.vkcode) {
			case VK_LEFT:leftwalk = false; break;
			case VK_RIGHT:rightwalk = false;break;
			case VK_UP:jumpwalk = false;singlejump = true;break;
			case VK_DOWN:downwalk = false;break;
			case VK_NUMPAD1:single_b_s = true;break;
			}
		}
	}
	if (bullet_shot_timer) bullet_shot_timer++;
	if (reb->timer) reb->timer++;
	if(bullet_shot_timer >= 100){single_b_s = true;}
	if (reb->isParrying) {
		single_b_s = false;
		bullet_shot_timer = 0;
	}
	else if (bullet_shot_timer > 0) {
		reb->isParrying = false;
	}
}

void player::Move() {
	if (leftwalk) vx -= speed;
	if (rightwalk) vx += speed;
	if (jumpwalk && jumpfreq && singlejump) {
		vy = -jumpspeed;
		jumpfreq--;
		jumpwalk = false;
		singlejump = false;
	}
	if (downwalk) vy = 1.0;

	if (vx > maxSpeed) vx = maxSpeed;
	if (vx < -maxSpeed) vx = -maxSpeed;

	y += vy;
	x += vx;

	if (!leftwalk && !rightwalk) {
		if (vx > friction) vx -= friction;
		else if (vx < -friction) vx += friction;
		else vx = 0;
	}

	if (vx > 0.1) player_p = false;
	else if (vx < -0.1) player_p = true;

	if (player_p) player_pic_idx = player_left[current_idx];
	else player_pic_idx = player_right[current_idx];
}

void player::Shot() {
	if (bullet_shot_timer > 0 && single_b_s && bullet_amount > 0) {
		mciSendString(L"play shoot from 0", NULL, 0, NULL);
		shotbullet(x, y, player_p, choose, bullet_shot_timer, bullet_list);
		single_b_s = false;
		bullet_shot_timer = 0;
		bullet_amount--;
	}
}

void player::Rebound(player& enemy) {
	if (goldtimer) reb->isParrying = true;
	if (!reb->isParrying) return;
	reb->parryDuration--;
	if (reb->parryDuration < 0) {
		reb->isParrying = false;
		reb->parryDuration = reb->parryDuration_max;
	}

	for (auto& b : enemy.bullet_list) {
		if (b.isPlusShot) {
			reb->check(*this, b);
			reb->bullet_rebound(b);
			if (reb->canParrying) {
				this->bullet_list.push_back(b);
				std::swap(b, enemy.bullet_list.back());
				enemy.bullet_list.pop_back();
			}
		}
	}
}

void player::Hurt(player& enemy) {
	for (auto& b : enemy.bullet_list) {
		if (b.isPlusShot) {
			if (distance(b.b_x, b.b_y, x, y) <= (double)b.plus_b_r + (double)r) {
				mciSendString(L"play hit", NULL, 0, NULL);
				HP -= plusBulletDamage;
				vx += b.getdir() ? -PlusRepelSpeed : PlusRepelSpeed;
				std::swap(b, enemy.bullet_list.back());
				enemy.bullet_list.pop_back();
			}
		}
		else {
			if (distance(b.b_x, b.b_y, x, y) < (double)b.b_r + (double)r) {
				mciSendString(L"play hit from 0", NULL, 0, NULL);
				HP -= bulletDamage;
				x += b.getdir() ? -repelamount : repelamount;
				std::swap(b, enemy.bullet_list.back());
				enemy.bullet_list.pop_back();
			}
		}
	}
}

void player::Live() {
	if (HP <= 0) { isDeath = true; gameover = true; }
	if (y > GameH + 300) { HP = 0; isDeath = true; gameover = true; }
}

void player::Counttimer() {
	if (counter++ == 3) current_idx++;
	counter %= 4;
	current_idx %= player_idx;
}

void player::Draw() {
	setfillcolor(WHITE);
	int ix = (int)x;
	int iy = (int)y;

	// 1. UI 绘制
	if (choose == 1) {
		Alpha_putimage(30, GameH - 170, avatar_paimon);
		HP_x = 110 + (int)(200.0 * HP / HP_max);
		setfillcolor(LIGHTRED);
		solidroundrect(110, GameH - 100, HP_x, GameH - 130, 10, 10);
		settextcolor(BLACK);
		settextstyle(30, 0, L"Comic Sans MS");
		outtextxy(HP_x - 30, GameH - 130, std::to_wstring(HP).c_str());
		Alpha_putimage(278, GameH - 78, normal_arrow);
		setfillcolor(BLACK);
		solidroundrect(30, GameH - 51, 278, GameH - 71, 2, 2);
		bulletfilling_x = 30 + (int)(248.0 * bulletFillingtimer / bulletFillingdelay);
		setfillcolor(LIGHTBLUE);
		solidroundrect(30, GameH - 51, bulletfilling_x, GameH - 71, 2, 2);
		settextcolor(WHITE);
		outtextxy(278 - 20, GameH - 80, (std::to_wstring(bullet_amount) + L" 发 ").c_str());
	}
	else if (choose == 2) {
		Alpha_putimage(GameW - 114, GameH - 170, avatar_nut);
		HP_x = GameW - 110 - (int)(200.0 * HP / HP_max);
		setfillcolor(LIGHTRED);
		solidroundrect(HP_x, GameH - 100, GameW - 110, GameH - 130, 10, 10);
		settextcolor(BLACK);
		settextstyle(30, 0, L"Comic Sans MS");
		outtextxy(HP_x + 5, GameH - 130, std::to_wstring(HP).c_str());
		Alpha_putimage(GameW - 310, GameH - 78, normal_arrow);
		setfillcolor(BLACK);
		solidroundrect(GameW - 278, GameH - 51, GameW - 30, GameH - 71, 2, 2);
		bulletfilling_x = GameW - 30 - (int)(248.0 * bulletFillingtimer / bulletFillingdelay);
		setfillcolor(LIGHTBLUE);
		solidroundrect(bulletfilling_x, GameH - 51, GameW - 30, GameH - 71, 2, 2);
		settextcolor(WHITE);
		outtextxy(GameW - 278, GameH - 80, (L" shot " + std::to_wstring(bullet_amount)).c_str());
	}

	// 2. 阴影
	if (isAboveGround && shadow_y != 2147483647) {
		Alpha_putimage(shadow_x, shadow_y, shadow[shadow_idx]);
	}

	// 3. 角色与特效
	if (enter_player_cursor_timer) {
		enter_player_cursor_timer--;
		if (choose == 1) Alpha_putimage(ix - location_r - 10, iy - location_r - 100, P1);
		else if (choose == 2) Alpha_putimage(ix - location_r - 10, iy - location_r - 100, P2);
	}

	if (!isDeath) {
		if (choose == 2 && vx != 0) {
			if (vx > 0) Alpha_putimage(ix - location_r, iy - location_r, nut_run_right[current_idx]);
			else if (vx < -0) Alpha_putimage(ix - location_r, iy - location_r, nut_run_left[current_idx]);
		}
		else {
			Alpha_putimage(ix - location_r, iy - location_r, player_pic_idx);
		}

		if (choose == 1) {
			if (player_p) Alpha_putimage(ix - location_r - 20, iy - 20, bow_pulling_left[weaponState_idx]);
			else Alpha_putimage(ix - location_r + 40, iy - 20, bow_pulling_right[weaponState_idx]);
		}
		else if (choose == 2) {
			if (player_p) Alpha_putimage(ix - location_r - 10, iy - 20, crossbow_pulling_left[weaponState_idx]);
			else Alpha_putimage(ix - location_r + 50, iy - 20, crossbow_pulling_right[weaponState_idx]);
		}
	}

	if (bullet_shot_timer > 12) Alpha_putimage(ix - location_r + (choose == 2 ? 35 : 32), iy - location_r - 50, alert);
	if (reb->isParrying) {
		int offset = (player_p ? -60 : 40);
		Alpha_putimage(ix - location_r + offset, iy - location_r - (choose == 2 ? 20 : 10), (choose == 2 ? iron_door : signboard));
	}
	if (goldtimer > 0) {
		goldtimer--;
		Alpha_putimage(ix - location_r + (choose == 2 ? 25 : 20), iy - location_r - 30, golden_chestplate);
	}

	// 4. 结算动画逻辑
	if (isDeath) {
		if (choose == 1) P_winner_idx = P_winner[1];
		else if (choose == 2) P_winner_idx = P_winner[0];
		WinnerAnimation = true;
	}

	if (WinnerAnimation) {
		if (overgameAnimation_timer > 0) overgameAnimation_timer--;
		
		// 计算动画偏移量 (0.0 -> 1.0)
		double anim_ratio = (double)(overgameAnimation_delay - overgameAnimation_timer) / overgameAnimation_delay;
		
		// 横幅滑动 (Y 从 0 到 150)
		int banner_y = (int)(150.0 * anim_ratio);
		Alpha_putimage(GameW / 2 - 323, banner_y, P_winner_idx);
		
		// 提示文字滑动 (从底部向上滑入)
		int text_y = GameH - (int)(260.0 * anim_ratio);
		settextcolor(BLACK);
		settextstyle(60, 20, L"微软雅黑");
		outtextxy(GameW / 2 - 500, text_y, L" 按下 E 键返回主菜单 ");
		outtextxy(GameW / 2 + 50, text_y, L" 按下 Q 键退出游戏 ");
	}

	// 5. 尸体显示
	if (isDeath) {
		if (choose == 1) Alpha_putimage(ix - location_r, iy - 10, player_p ? paimon_die[0] : paimon_die[1]);
		else if (choose == 2) Alpha_putimage(ix - location_r, iy - location_r, player_p ? nut_die[0] : nut_die[1]);
	}
}

void player::Reset() {
	x = temp_x; y = temp_y; vx = vy = 0; jumpfreq = 2; HP = PlayerHP;
	bullet_list.clear(); reb->Reset(); isDeath = false; bullet_shot_timer = 0;
	player_p = true; singlejump = true; jumpwalk = downwalk = rightwalk = leftwalk = false;
	single_b_s = single_reb = false;
	enter_player_cursor_timer = enter_player_cursor_delay;
	overgameAnimation_timer = overgameAnimation_delay;
	WinnerAnimation = false; counter = 0; goldtimer = 0; current_idx = 0; gameover = false;
}

void player::BulletFilling() {
	if (bullet_amount < maxBulletAmount) bulletFillingtimer++;
	if (bulletFillingtimer == bulletFillingdelay) {
		bullet_amount++; bulletFillingtimer = 0;
	}
}
