#include<graphics.h>
#include<iostream>
#include<windows.h>
#include<vector>
#include<string>
#include<cmath>
#include<conio.h>
#include"scene.h"
#include"menuScene.h"
#include"GameScene.h"
#include"scene_manager.h"
#include"util.h"
#include"Atlas.h"
using namespace std;

#pragma comment(lib,"MSIMG32.LIB")
inline void Alpha_putimage(int x,int y,IMAGE img) {
	int w = img.getwidth();
	int h = img.getheight();
	AlphaBlend(GetImageHDC(), x, y, w, h, GetImageHDC(&img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

void flip_atlas(Atlas& src, Atlas& dst) {
	dst.clear();
	for (int i = 0; i < src.get_size(); i++) {
		IMAGE img_flpipped;
		flip_image(src.get_image(i), &img_flpipped);
		dst.add_image(img_flpipped);
	}
}

SceneManager scene_manager;

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
IMAGE menu_background;
IMAGE game_background;
const int plus_time = 20;
const int g = 3;
const int GameW = 1500;
const int GameH = 800;
const int PlayerHP = 20;

bool running = true;
bool gamerunning = false;
bool gameover = false;
bool gamefinished = false;

class player;
class bullet;
class rebound;

void shotbullet(int& x, int& y, bool& dir, int bst, vector<bullet>& bullet_list);
void updateGameEnvironment(player& p);
void Show(player& p);

void loadOtherAnimation() {
	loadimage(&menu_background, L"img/background.png",GameW,GameH);
	loadimage(&game_background, L"img/battlebackground.png", GameW, GameH);
}

class bullet {
	friend void updatebullet(vector<bullet>& bullet_list);
	friend class rebound;
public:
	bullet(int x, int y, bool dir, int b_s_time) :b_x(x), b_y(y), bullet_shot_time(b_s_time), bullet_dir(dir) {}

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
		}
		else {
			setfillcolor(WHITE);
			solidcircle(b_x, b_y, b_r);
		}

	}

	bool getdir() {
		return bullet_dir;
	}

	bool isPlusShot = false;
	int b_x;
	int b_y;
	int b_r = 5;
	int plus_b_r = 25;
private:
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
		if (bullet_list[i].b_x < 0 || bullet_list[i].b_x>GameW) {
			swap(bullet_list[i], bullet_list.back());
			bullet_list.pop_back();
			continue;
		}
		bullet_list[i].move();
		bullet_list[i].check();
		bullet_list[i].draw();
	}
}

double distance(int x1, int y1, int x2, int y2) {
	double d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	return d;
}
//Õñµ¶
class rebound {
public:

	void check(player& p, bullet& b); 

	void parryCooldown() {
		if (timer >= t&&!isParrying) {
			isParrying = true;
			timer = 0;
		}
	}

	void bullet_rebound(bullet& b) {
		if (canParrying) {
			cout << "·´µ¯£¡" << endl;
			b.bullet_speed += 20;
			bool temp = b.bullet_dir;
			b.bullet_dir = temp?false:true;
		}

	}

	void Reset() {
		isParrying = false;
		canParrying = false;
	}

	bool isParrying = false;
	bool canParrying = false;
	int parryDuration = 10;
	int timer = t;
	const int t = 3;
private:
	int parryWindow = 10;
};

class player {
public:
	player(int x1, int y1, int r1,wstring s1,int choose1): player_left(player_idx),player_right(player_idx),player_pic_idx(player_left[0]),x(x1),y(y1),r(r1),choose(choose1),temp_x(x1),temp_y(y1)
	{
		loadAnimation(s1);
		reb = new rebound();
	}

	~player() {
		delete reb;
	}
public:
	void loadAnimation(wstring s) {
		for (int i = 0;i < player_idx;i++) {
			wstring path_left = L"img/"+ s +L"_left_" + to_wstring(i) + L".png";
			wstring path_right = L"img/"+ s +L"_right_" + to_wstring(i) + L".png";
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
					case'K':if (!reb->timer)reb->timer = 1;reb->parryCooldown();break;
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
		}
		else if (choose == 2) {
				if (m.message == WM_KEYDOWN) {
					switch (m.vkcode) {
					case VK_LEFT:leftwalk = true;break;
					case VK_RIGHT:rightwalk = true;break;
					case VK_UP:jumpwalk = true;break;
					case VK_DOWN:downwalk = true;break;
					case'M':bullet_shot_timer++;break;
					case'N':if (!reb->timer)reb->timer = 1;reb->parryCooldown();break;
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
		}
		if (bullet_shot_timer)bullet_shot_timer++;//ÐîÁ¦Ê±¼ä
		if(reb->timer)reb->timer++;

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

		//ÒÆ¶¯
		y += vy;
		x += vx;

		//×ªÏò
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

		if (abs(vx) > maxSpeed) {
			vx = vx > 0 ? maxSpeed : -maxSpeed;
		}
	}

	void Shot() {
		if (bullet_shot_timer > 0 && single_b_s) {
			shotbullet(x, y, player_p, bullet_shot_timer,bullet_list);
			single_b_s = false;
			bullet_shot_timer = 0;
		}

		if (bullet_shot_timer >= plus_time) { ifplusattack = true; }
	}

	void Rebound(player& enemy) {
		if (goldtimer)reb->isParrying = true;
		if (!reb->isParrying) { return; }
		reb->parryDuration--;
		if (reb->parryDuration < 0) { 
			reb->isParrying = false; 
			reb->parryDuration = 10;
		}


		for (auto& b : enemy.bullet_list) {
			if (b.isPlusShot) {
				reb->check(*this, b);
				reb->bullet_rebound(b);

				if (reb->canParrying) {
					this->bullet_list.push_back(b);
					swap(b, enemy.bullet_list.back());
					enemy.bullet_list.pop_back();
				}
			}
		}
	}

	void Hurt(player& enemy) {
		for (auto& b : enemy.bullet_list) {
			if (b.isPlusShot) {
				if (distance(b.b_x, b.b_y, x, y) < b.plus_b_r + r) {
					HP -= 3;
					vx += b.getdir() ? -20 : 20;
					swap(b, enemy.bullet_list.back());
					enemy.bullet_list.pop_back();
				}
			}
			else {
				if (distance(b.b_x, b.b_y, x, y) < b.b_r + r) {
					HP--;
					x += b.getdir() ? -20 : 20;
					swap(b, enemy.bullet_list.back());
					enemy.bullet_list.pop_back();
				}
			}
		}
	}

	void Live() {
		if (HP <= 0) {
			isDeath = true;
			gameover = true;
		}
	}

	void Counttimer() {
		if (counter++ == 3)current_idx++;
		counter %= 4;
		current_idx %= player_idx;
	}
	
	void Draw() {
		Show(*this);

		Alpha_putimage(x, y, player_pic_idx);
		setfillcolor(WHITE);
		if(choose==1){ solidcircle(x, y, r); }
		else if (choose == 2) { solidrectangle(x - r, y - r, x + r, y + r); }

		if (ifplusattack) {
			setfillcolor(BLUE);
			solidrectangle(x - r / 2, y - r / 2, x + r / 2, y + r / 2);
			ifplusattack = false;
		}

		if (reb->isParrying) {
			setfillcolor(LIGHTRED);
			solidcircle(x, y, r / 3);
		}

		if (reb->canParrying||goldtimer) {
			if (reb->canParrying) {
				goldtimer = 100;
				reb->canParrying = false;
			}
			setfillcolor(RGB(255, 215, 0));
			solidcircle(x, y, 2 * r);
			goldtimer--;
		}

		if (isDeath) {
			settextcolor(WHITE);
			settextstyle(150, 50, _T("Î¢ÈíÑÅºÚ"));
			wstring s = L"Íæ¼Ò" + to_wstring(choose) + L"Ê§°Ü";
			outtextxy(GameW / 2 - 250, GameH / 2 - 75, s.c_str());
		}
	}

	void Reset() {
		x = temp_x;
		y = temp_y;
		vx = vy = 0;
		jumpfreq = 2;
		HP = PlayerHP;
		bullet_list.clear();
		reb->Reset();
		isDeath = false;
	}


	int x ;
	int y ;
	int r ;
	int vy = 0;
	int vx = 0;
	int jumpfreq = 2; 
	vector<bullet> bullet_list;
	rebound* reb;
	int choose;
	int HP = PlayerHP;
private:
	int temp_x, temp_y;

	const int speed = 3;
	const int maxSpeed = 10;

	int goldtimer = 0;

	const int player_idx = 6;
	vector<IMAGE> player_left;
	vector<IMAGE> player_right;
	IMAGE player_pic_idx;
	//¼ÆÊ±Æ÷
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

	bool ifplusattack = false;

	bool isDeath = false;

};

void Show(player& p) {
	if (p.choose == 1) {
		settextcolor(RED);
		settextstyle(40, 15, _T("Î¢ÈíÑÅºÚ"));
		wstring s = L"P1 HP£º" + to_wstring(p.HP);
		outtextxy(0, 0, s.c_str());
	}
	else if (p.choose == 2) {
		settextcolor(LIGHTBLUE);
		settextstyle(40, 15, _T("Î¢ÈíÑÅºÚ"));
		wstring s = L"P2 HP£º" + to_wstring(p.HP);
		outtextxy(GameW-200, 0, s.c_str());
	}
}

void updateplayer(player& p,player& enemy) {
	p.Move();
	updateGameEnvironment(p);
	p.Rebound(enemy);
	p.Shot();
	p.Hurt(enemy);
	p.Live();
	p.Counttimer();
	p.Draw();
}

void rebound::check(player& p, bullet& b) {
	{
		if (isParrying) {
			if (parryWindow > 0) {
				parryWindow--;
				if (distance(b.b_x+10, b.b_y, p.x, p.y) < b.plus_b_r + p.r) {
					p.reb->canParrying = true;cout << "can" << endl;
				}
			}
			else { parryWindow = 10; }
		}
	}
}

class environment {
public:
	environment(player& p) : x(p.x),y(p.y),r(p.r),vy(p.vy),vx(p.vx),jumpfreq(p.jumpfreq){}

	void gravity(player& p) {
		if (y + r < GameH-100) { p.vy += g; }
		else if (y + r + vy >= GameH-100) {
			p.vy = 0;
			p.y = GameH - 100 - r;
			p.jumpfreq = 2;
		}
	}

	void wall(player& p) {
		if (x + r >= GameW-100 || x - r <= 100) {
			p.vx = -vx;
		}
		if (x + r >= GameW - 100) {
			p.x = GameW - 100 - r;
		}
		if (x - r <= 100) {
			p.x = 100 + r;
		}
	}

	void Draw() {
		line(0, GameH - 100, GameW, GameH - 100);
		line(100, GameH - 100, 100, 100);
		line(GameW - 100, GameH - 100, GameW - 100, 100);
	}
	
private:
	int x;
	int y;
	int r;
	int vy ;
	int vx ;
	int jumpfreq ;

};

void updateGameEnvironment(player& p) {
	environment envir(p);
	envir.gravity(p);
	envir.wall(p);
	envir.Draw();
}

void updateGame(player& p1,player& p2) {
	updateplayer(p1,p2);
	updateplayer(p2,p1);
	updatebullet(p1.bullet_list);
	updatebullet(p2.bullet_list);
}

void ResetGame(player& p1,player& p2) {
	p1.Reset();
	p2.Reset();
}

int main() {
	
	initgraph(GameW, GameH);

	player p1(500, 400, 30, L"paimon", 1);
	player p2(800, 400, 30, L"boar", 2);
	loadOtherAnimation();

	ExMessage msg;
	menu_scene = new MenuScene();
	game_scene = new GameScene();

	setbkmode(TRANSPARENT);

	scene_manager.set_current_scene(menu_scene);

	BeginBatchDraw();
	while (running) {
		DWORD start_time = GetTickCount();

		if (gamefinished) {
			ResetGame(p1, p2);
			gamefinished = false;
		}

		while (peekmessage(&msg))
		{
			p1.processdeal(msg);
			p2.processdeal(msg);
			scene_manager.on_input(msg);
		}

		scene_manager.on_update();

		cleardevice();
		scene_manager.on_draw();
		if(gamerunning)updateGame(p1,p2);

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