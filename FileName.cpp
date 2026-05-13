#include<graphics.h>
#include<iostream>
#include<windows.h>
#include<vector>
#include<string>
#include<cmath>
using namespace std;

#pragma comment(lib,"MSIMG32.LIB")
inline void Alpha_putimage(int x, int y, IMAGE img) {
	int w = img.getwidth();
	int h = img.getheight();
	AlphaBlend(GetImageHDC(), x, y, w, h, GetImageHDC(&img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

const int speed = 2;
const int plus_time = 20;
const int g = 3;


class player;
class bullet;

void shotbullet(int& x, int& y, bool& dir, int bst, vector<bullet>& bullet_list);
void updateGameEnvironment(player& p);



class player {
public:
	player(int x1, int y1, int r1, wstring s1, int choose1) : player_left(player_idx), player_right(player_idx), player_pic_idx(player_left[0])
	{
		this->x = x1;
		this->y = y1;
		this->r = r1;
		this->choose = choose1;
		loadAnimation(s1);
	}
public:
	void loadAnimation(wstring s) {
		for (int i = 0;i < player_idx;i++) {
			wstring path_left = L"img/" + s + L"_left_" + to_wstring(i) + L".png";
			wstring path_right = L"img/" + s + L"_right_" + to_wstring(i) + L".png";
			loadimage(&player_left[i], path_left.c_str());
			loadimage(&player_right[i], path_right.c_str());
		}
	}


	void processdeal(ExMessage m) {
		if (choose == 1) {
			if (m.message == WM_KEYDOWN) {
				switch (m.vkcode) {
				case'A':leftwalk = true;break;
				case'D':rightwalk = true;break;
				case'W':jumpwalk = true;break;
				case'S':downwalk = true;break;
				case'J':bullet_shot_timer++;break;
				}
			}
			if (m.message == WM_KEYUP) {
				switch (m.vkcode) {
				case'A':leftwalk = false;break;
				case'D':rightwalk = false;break;
				case'W':jumpwalk = false;singlejump = true;break;
				case'S':downwalk = false;break;
				case'J':single_b_s = true;break;
				}
			}
			if (bullet_shot_timer)bullet_shot_timer++;//蓄力时间
		}
		else if (choose == 2) {
			if (m.message == WM_KEYDOWN) {
				switch (m.vkcode) {
				case VK_LEFT:leftwalk = true;break;
				case VK_RIGHT:rightwalk = true;break;
				case VK_UP:jumpwalk = true;break;
				case VK_DOWN:downwalk = true;break;
				case'M':bullet_shot_timer++;break;
				}
			}
			if (m.message == WM_KEYUP) {
				switch (m.vkcode) {
				case VK_LEFT:leftwalk = false; break;
				case VK_RIGHT:rightwalk = false;break;
				case VK_UP:jumpwalk = false;singlejump = true;break;
				case VK_DOWN:downwalk = false;break;
				case'M':single_b_s = true;break;
				}
			}
			if (bullet_shot_timer)bullet_shot_timer++;
		}


	}

	void Move() {
		if (leftwalk) {
			vx -= speed;
		}
		if (rightwalk) {
			vx += speed;
		}
		if (jumpwalk && jumpfreq && singlejump) {
			vy = -30;
			jumpfreq--;
			jumpwalk = false;
			singlejump = false;
		}
		if (downwalk) {
			vy = 1;
			y += 100;
		}

		//移动
		y += vy;
		x += vx;

		//转向
		if (vx > 0) {
			vx--;
			player_p = false;
		}
		else if (vx < 0) {
			vx++;
			player_p = true;
		}
		if (player_p) { player_pic_idx = player_left[current_idx]; }
		else { player_pic_idx = player_right[current_idx]; }
	}

	void Shot() {
		if (bullet_shot_timer > 0 && single_b_s) {
			shotbullet(x, y, player_p, bullet_shot_timer, bullet_list);
			single_b_s = false;
			bullet_shot_timer = 0;
		}

		if (bullet_shot_timer >= plus_time) { ifplusattack = true; }
	}


	void Counttimer() {
		if (counter++ == 3)current_idx++;
		counter %= 4;
		current_idx %= player_idx;
	}

	void Draw() {
		Alpha_putimage(x, y, player_pic_idx);
		setfillcolor(WHITE);
		if (choose == 1) { solidcircle(x, y, r); }
		else if (choose == 2) { solidrectangle(x - r, y - r, x + r, y + r); }

		if (ifplusattack) {
			setfillcolor(BLUE);
			solidrectangle(x - r / 2, y - r / 2, x + r / 2, y + r / 2);
			ifplusattack = false;
		}

	}


	int x;
	int y;
	int r;
	int vy = 0;
	int vx = 0;
	int jumpfreq = 2;
	vector<bullet> bullet_list;
	int choose;

private:

	const int player_idx = 6;
	vector<IMAGE> player_left;
	vector<IMAGE> player_right;
	IMAGE player_pic_idx;
	//计时器
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

	bool ifplusattack = false;

};

void updateplayer(player& p) {
	p.Move();
	updateGameEnvironment(p);
	p.Shot();
	p.Counttimer();
	p.Draw();
}

class bullet {
	friend void updatebullet(vector<bullet>& bullet_list);
	friend class rebound;
public:
	bullet(int x, int y, bool dir, int b_s_time) {
		this->b_x = x;
		this->b_y = y;
		this->bullet_shot_time = b_s_time;
		this->bullet_dir = dir;
	}
	void move() {
		if (bullet_dir) {
			b_x -= bullet_speed;
		}
		else {
			b_x += bullet_speed;
		}
	}
	void check() {
		if (bullet_shot_time < plus_time) {
			isPlusShot = false;
		}
		else { isPlusShot = true; }
	}
	void draw() {
		if (isPlusShot) {
			setfillcolor(LIGHTBLUE);
			solidcircle(b_x, b_y, plus_b_r);
			isPlusShot = false;
		}
		else {
			setfillcolor(WHITE);
			solidcircle(b_x, b_y, b_r);
		}

	}

	bool isPlusShot = false;

private:
	int b_x;
	int b_y;
	int b_r = 5;
	int plus_b_r = 25;
	int bullet_shot_time;
	bool bullet_dir;

	int bullet_speed = 30;
};

void shotbullet(int& x, int& y, bool& dir, int bst, vector<bullet>& bullet_list) {
	bullet b(x, y, dir, bst);
	bullet_list.push_back(b);
}

void updatebullet(vector<bullet>& bullet_list) {
	for (size_t i = 0;i < bullet_list.size();i++) {
		if (bullet_list[i].b_x < 0 || bullet_list[i].b_x>1200) {
			swap(bullet_list[i], bullet_list.back());
			bullet_list.pop_back();
			continue;
		}
		bullet_list[i].move();
		bullet_list[i].check();
		bullet_list[i].draw();
	}
}

class environment {
public:
	environment(player& p) : x(p.x), y(p.y), r(p.r), vy(p.vy), vx(p.vx), jumpfreq(p.jumpfreq) {}

	void gravity(player& p) {
		if (y + r < 600) { p.vy += g; }
		else if ((y + r + vy >= 600)) {
			p.vy = 0;
			p.y = 600 - r;
			p.jumpfreq = 2;
		}
	}

	void wall(player& p) {
		if (x + r >= 1100 || x - r <= 100) {
			p.vx = -vx;
		}
		if (x + r >= 1100) {
			p.x = 1100 - r;
		}
		if (x - r <= 100) {
			p.x = 100 + r;
		}
	}

private:
	int x;
	int y;
	int r;
	int vy;
	int vx;
	int jumpfreq;

};

void updateGameEnvironment(player& p) {
	environment envir(p);
	envir.gravity(p);
	envir.wall(p);
}

void updateGame(player& p1, player& p2) {
	updateplayer(p1);
	updateplayer(p2);
	updatebullet(p1.bullet_list);
	updatebullet(p2.bullet_list);
}

int main() {

	initgraph(1200, 800);

	player p1(500, 400, 30, L"paimon", 1);
	player p2(800, 400, 30, L"boar", 2);

	ExMessage msg;

	BeginBatchDraw();
	while (1) {
		DWORD start_time = GetTickCount();


		cleardevice();
		while (peekmessage(&msg))
		{
			p1.processdeal(msg);
			p2.processdeal(msg);
		}

		updateGame(p1, p2);


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