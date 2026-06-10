#include<graphics.h>
#include<iostream>
#include<windows.h>
#include<mmsystem.h>
#include<vector>
#include<string>
#include<cmath>
#include<conio.h>
#include<timeapi.h>
#include"scene.h"

#pragma comment(lib,"Winmm.lib")

// 高精度计时器
LARGE_INTEGER frequency;
void InitTimer() { QueryPerformanceFrequency(&frequency); }
double GetTime() {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return (double)counter.QuadPart / frequency.QuadPart;
}
#include"menuScene.h"
#include"gameScene.h"
#include"sceneManager.h"
#include"util.h"
#include"map.h"
#include"map1.h"
#include"map2.h"
#include"mapManager.h"
#include"player.h"
#include"bullet.h"
#include"rebound.h"
using namespace std;

#pragma comment(lib,"Winmm.lib")
SceneManager scene_manager;
MapManager map_manager;

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;

Map* map1 = nullptr;
Map* map2 = nullptr;

vector<IMAGE> startGameButton(3);
vector<IMAGE> exitGameButton(3);
vector<IMAGE> mapChangeButton(2);
vector<IMAGE> gameMaps(2);
vector<IMAGE> shadow(2);
vector<IMAGE> arrow(2);
vector<IMAGE> spectral_arrow(2);
vector<IMAGE> firework(2);

vector<IMAGE> nut_run_left(3);
vector<IMAGE> nut_run_right(3);
vector<IMAGE> nut_die(2);
vector<IMAGE> paimon_die(2);

vector<IMAGE> bow_pulling_left(4);
vector<IMAGE> bow_pulling_right(4);
vector<IMAGE> crossbow_pulling_left(5);
vector<IMAGE> crossbow_pulling_right(5);

vector<IMAGE> P_winner(2);
IMAGE P_winner_idx;

IMAGE GameMap;
IMAGE iron_door;
IMAGE signboard;
IMAGE P1;
IMAGE P2;
IMAGE alert;
IMAGE golden_chestplate;
IMAGE avatar_paimon;
IMAGE avatar_nut;
IMAGE normal_arrow;


const int g = 3;
const int GameW = 1600;
const int GameH = 900;
const int startGameButtonW = 500;
const int startGameButtonH = 130;
const int exitGameButtonW = 500;
const int exitGameButtonH = 130;
const int mapChangeButtonW = 65;
const int mapChangeButtonH = 180;

int map_index = 0;

int bkg_x = -224;
int bkg_y = 0;
int bk_w = 2048;
int bk_h = 1440;

bool running = true;
bool gamerunning = false;
bool gameover = false;
bool gameRestart = false;



void loadOtherAnimation() {
	loadimage(&normal_arrow, L"img/normal_arrow.png");

	loadimage(&P_winner[0], L"img/1P_winner.png");
	loadimage(&P_winner[1], L"img/2P_winner.png");

	loadimage(&avatar_paimon, L"img/avatar_paimon.png");
	loadimage(&avatar_nut, L"img/avatar_nut.png");

	loadimage(&golden_chestplate, L"img/golden_chestplate.png", 50, 50);

	loadimage(&alert, L"img/alert.png");

	loadimage(&P1, L"img/1P_cursor.png", 100, 100);
	loadimage(&P2, L"img/2P_cursor.png", 100, 100);

	loadimage(&iron_door, L"img/iron_door.png",120,120);
	loadimage(&signboard, L"img/birch_sign.png",120,120);


	for (int i = 0;i < 5;i++) {
		wstring s1 = L"img/crossbow_pulling_" + to_wstring(i) + L".png";
		wstring s2 = L"img/crossbow_pulling_right_" + to_wstring(i) + L".png";
		loadimage(&crossbow_pulling_left[i], s1.c_str(),60,60);
		loadimage(&crossbow_pulling_right[i], s2.c_str(),60,60);
	}

	for (int i = 0;i < 4;i++) {
		wstring s1 = L"img/bow_pulling_" + to_wstring(i) + L".png";
		wstring s2 = L"img/bow_pulling_right_" + to_wstring(i) + L".png";
		loadimage(&bow_pulling_left[i], s1.c_str(),50,50);
		loadimage(&bow_pulling_right[i], s2.c_str(),50,50);
	}

	loadimage(&paimon_die[0], L"img/paimon_die_left.png");
	loadimage(&paimon_die[1], L"img/paimon_die_right.png");

	loadimage(&nut_die[0], L"img/nut_die_left.png");
	loadimage(&nut_die[1], L"img/nut_die_right.png");

	for (int i = 0;i < 3;i++) {
		wstring s1 = L"img/nut_run_left_" + to_wstring(i + 1) + L".png";
		wstring s2 = L"img/nut_run_right_" + to_wstring(i + 1) + L".png";
		loadimage(&nut_run_left[i], s1.c_str());
		loadimage(&nut_run_right[i], s2.c_str());
	}

	loadimage(&firework[0], L"img/firework_rocket.png", 100, 100);
	loadimage(&firework[1], L"img/firework_rocket_right.png", 100, 100);

	loadimage(&spectral_arrow[0], L"img/spectral_arrow.png",130,130);
	loadimage(&spectral_arrow[1], L"img/spectral_arrow_right.png", 130, 130);

	loadimage(&arrow[0], L"img/arrow.png",60,60);
	loadimage(&arrow[1], L"img/arrow_right.png",60,60);

	for (int i = 0;i < 2;i++) {
		wstring s = L"img/shadow_" + to_wstring(i + 1) + L".png";
		loadimage(&shadow[i], s.c_str());
	}

	for (int i = 0;i < 2;i++) {
		wstring s = L"img/map" + to_wstring(i + 1) + L"_bg.png";
		loadimage(&gameMaps[i], s.c_str(),GameW,GameH);
	}


	for (int i = 0;i < 3;i++) {
		wstring s1 = L"img/startGame_" + to_wstring(i + 1) + L".png";
		wstring s2 = L"img/exitGame_" + to_wstring(i + 1) + L".png";
		loadimage(&startGameButton[i], s1.c_str(), startGameButtonW, startGameButtonH);
		loadimage(&exitGameButton[i], s2.c_str(), exitGameButtonW, exitGameButtonH);
	}

	for (int i = 0;i < 2;i++) {
		wstring s = L"img/mapChange_" + to_wstring(i + 1) + L".png";
		loadimage(&mapChangeButton[i], s.c_str(), mapChangeButtonW, mapChangeButtonH);
	}
}

void logic_updateplayer(player& p, player& enemy) {
	if (!p.isDeath) {
		p.Move();
		p.Rebound(enemy);
		p.BulletFilling();
		p.Shot();
		p.Hurt(enemy);
		p.Live();
		p.Counttimer();
	}
}

void renderplayer(player& p) {
	p.Draw();
}


void logic_updateGame(player& p1, player& p2) {
	map_manager.on_update(p1);
	map_manager.on_update(p2);

	logic_updateplayer(p1, p2);
	logic_updateplayer(p2, p1);

	movebullet(p1.bullet_list);
	movebullet(p2.bullet_list);
}

void renderGame(player& p1, player& p2) {
	renderplayer(p1);
	renderplayer(p2);

	drawbullets(p1.bullet_list);
	drawbullets(p2.bullet_list);
}

void ResetGame(player& p1, player& p2) {
	p1.Reset();
	p2.Reset();
}

void loadmusic() {
	mciSendString(L"open mus/bgm1.MP3 alias bgm1", NULL, 0, NULL);
	mciSendString(L"open mus/bgm2.MP3 alias bgm2", NULL, 0, NULL);
	mciSendString(L"open mus/bowhit.MP3 alias hit", NULL, 0, NULL);
	mciSendString(L"open mus/bowshoot.MP3 alias shoot", NULL, 0, NULL);
	mciSendString(L"open mus/iron_door_close.MP3 alias iron_door", NULL, 0, NULL);
	mciSendString(L"open mus/sign.MP3 alias signboard", NULL, 0, NULL);
	mciSendString(L"open mus/铁砧.MP3 alias rebound", NULL, 0, NULL);
	mciSendString(L"open mus/click_stereo.MP3 alias exitgame", NULL, 0, NULL);
	mciSendString(L"open mus/levelup.MP3 alias startgame", NULL, 0, NULL);
}

int main() {
	timeBeginPeriod(1);
	InitTimer();

	initgraph(GameW, GameH);
	loadmusic();

	player p1(370, 400, 40, L"paimon", 1, 6);
	player p2(1225, 400, 46, L"nut_idle", 2, 3);
	loadOtherAnimation();

	ExMessage msg;
	menu_scene = new MenuScene();
	game_scene = new GameScene();

	map1 = new Map1();
	map2 = new Map2();

	setbkmode(TRANSPARENT);

	scene_manager.set_current_scene(menu_scene);
	map_manager.set_current_map(map1);

	const double target_frame_time = 1.0 / 60.0;

	BeginBatchDraw();
	while (running) {
		double frame_start_time = GetTime();

		while (peekmessage(&msg))
		{
			p1.processdeal(msg);
			p2.processdeal(msg);
			scene_manager.on_input(msg);
		}

		if (gameRestart) {
			ResetGame(p1, p2);
			gameRestart = false;
		}
		scene_manager.on_update();
		if (gamerunning) {
			logic_updateGame(p1, p2);
		}

		scene_manager.on_draw();
		if (gamerunning) {
			renderGame(p1, p2);
		}

		FlushBatchDraw();
	
		double frame_end_time = GetTime();
		double delta_time = frame_end_time - frame_start_time;
		if (delta_time < target_frame_time) {
			DWORD sleep_ms = (DWORD)((target_frame_time - delta_time) * 1000);
			if (sleep_ms > 1) Sleep(sleep_ms - 1);
			while (GetTime() - frame_start_time < target_frame_time) {
				YieldProcessor();
			}
		}
	}
	EndBatchDraw();

	timeEndPeriod(1);
	return 0;
}