#include<graphics.h>
#include<iostream>
#include<windows.h>
#include<vector>
#include<string>
using namespace std;

#pragma comment(lib,"MSIMG32.LIB")
inline void Alpha_putimage(int x, int y, IMAGE img) {
	int w = img.getwidth();
	int h = img.getheight();
	AlphaBlend(GetImageHDC(), x, y, w, h, GetImageHDC(&img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

const int player_idx = 6;
const int speed = 5;
const int plus_time = 20;
const int g = 3;
vector<IMAGE> player_left(player_idx);
vector<IMAGE> player_right(player_idx);

class bullet {
	friend void updatebullet();
public:
	bullet(int x, int y, bool dir, int b_s_time) {
		this->b_x = x;
		this->b_y = y;
		this->bullet_shot_time = b_s_time;
		if (bullet_dir == NULL)this->bullet_dir = dir;
	}
	void move() {
		if (bullet_dir) {
			b_x -= bullet_speed;
		}
		else {
			b_x += bullet_speed;
		}
	}
	void draw() {
		if (bullet_shot_time < plus_time) {
			setfillcolor(WHITE);
			solidcircle(b_x, b_y, b_r);
		}
		else {
			setfillcolor(LIGHTBLUE);
			solidcircle(b_x, b_y, b_r * 5);
		}

	}
private:
	int b_x;
	int b_y;
	int b_r = 5;
	int bullet_shot_time;
	bool bullet_dir = NULL;
	int bullet_speed = 30;
};
vector<bullet> bullet_list;


void shotbullet(int& x, int& y, bool& dir, int bst) {
	bullet b(x, y, dir, bst);
	bullet_list.push_back(b);
}

void updatebullet() {
	for (size_t i = 0;i < bullet_list.size();i++) {
		if (bullet_list[i].b_x < 0 || bullet_list[i].b_x>1200) {
			swap(bullet_list[i], bullet_list.back());
			bullet_list.pop_back();
			continue;
		}
		bullet_list[i].move();
		bullet_list[i].draw();
	}
	//cout << bullet_list.size() << ' ';
}


void loadAnimation() {
	for (int i = 0;i < player_idx;i++) {
		wstring path_left = L"img/paimon_left_" + to_wstring(i) + L".png";
		wstring path_right = L"img/paimon_right_" + to_wstring(i) + L".png";
		loadimage(&player_left[i], path_left.c_str());
		loadimage(&player_right[i], path_right.c_str());
	}
}

void zhongli(int& y, int& vy, int r, int& jump) {
	if (y + r < 600) { vy += g; }
	else if ((y + r + vy >= 600)) {
		vy = 0;
		y = 600 - r;
		jump = 2;
	}
}

void pengzhuang(int& x, int& vx, int r) {
	if (x + r + vx >= 1100 || x - r + vx <= 100) {
		vx = -vx;
	}
}

int main() {

	initgraph(1200, 800);
	loadAnimation();

	int x = 600;
	int y = 400;
	int r = 30;
	int vy = 0;
	int vx = 0;
	int jump = 2;

	int current_player = 0;
	int counter = 0;

	bool player_p = true;

	IMAGE player = player_left[0];

	bool singlejump = true;
	bool jumpwalk = false;
	bool downwalk = false;
	bool rightwalk = false;
	bool leftwalk = false;

	int bul_shot_time = 0;
	bool single_b_s = false;

	bool ifplusattack = false;

	BeginBatchDraw();
	while (1) {
		DWORD start_time = GetTickCount();

		ExMessage m;
		while (peekmessage(&m)) {
			if (m.message == WM_KEYDOWN) {
				switch (m.vkcode) {
				case'A':leftwalk = true;break;
				case'D':rightwalk = true;break;
				case'W':jumpwalk = true;break;
				case'S':downwalk = true;break;
				case'J':bul_shot_time++;break;
				}
			}
			if (m.message == WM_KEYUP) {
				switch (m.vkcode) {
				case'A':leftwalk = false;break;
				case'D':rightwalk = false;break;
				case'W':jumpwalk = false;singlejump = true;break;
				case'S':downwalk = false;break;
				case'J':single_b_s = true;cout << "J已经松开" << ' ';break;
				}
			}
		}
		if (bul_shot_time)bul_shot_time++;

		if (leftwalk) {
			vx -= speed;
		}
		if (rightwalk) {
			vx += speed;
		}
		if (jumpwalk && jump && singlejump) {
			vy = -30;
			jump--;
			jumpwalk = false;
			singlejump = false;
		}
		if (downwalk) {
			vy = 1;
			y += 100;
		}

		zhongli(y, vy, r, jump);
		pengzhuang(x, vx, r);
		y += vy;
		x += vx;
		if (vx > 0) {
			vx--;
			player_p = false;
		}
		else if (vx < 0) {
			vx++;
			player_p = true;
		}

		//cout << "按的时间为：" << bul_shot_time << endl;
		if (bul_shot_time > 0 && single_b_s) {
			cout << "按的时间为：" << bul_shot_time << endl;
			shotbullet(x, y, player_p, bul_shot_time);
			single_b_s = false;
			bul_shot_time = 0;
		}

		if (bul_shot_time >= plus_time) { ifplusattack = true; }

		if (player_p) { player = player_left[current_player]; }
		else { player = player_right[current_player]; }

		if (counter++ == 3)current_player++;
		counter %= 4;
		current_player %= player_idx;


		cleardevice();
		putimage(x, y, &player);
		setfillcolor(WHITE);
		solidcircle(x, y, r);
		if (ifplusattack) {
			setfillcolor(BLUE);
			solidrectangle(x - r / 2, y - r / 2, x + r / 2, y + r / 2);
			ifplusattack = false;
		}
		updatebullet();
		line(0, 600, 1200, 600);
		line(100, 600, 100, 100);
		line(1100, 600, 1100, 100);
		FlushBatchDraw();

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;
		if (delta_time < 1000 / 60) {
			Sleep(1000 / 60 - delta_time);
		}
	}
	EndBatchDraw();

	return 0;
}