#pragma once
#include<vector>
#include<windows.h>	
#include"scene.h"
#include"sceneManager.h"
#include"util.h"
#include"mapManager.h"
using namespace std;

extern SceneManager scene_manager;

extern IMAGE GameMap;
extern vector<IMAGE> gameMaps;
extern vector<IMAGE> startGameButton;
extern vector<IMAGE> exitGameButton;
extern vector<IMAGE> mapChangeButton;

extern MapManager map_manager;

extern const int GameW;
extern const int GameH;
extern bool running;
extern const int startGameButtonW;
extern const int startGameButtonH;
extern const int exitGameButtonW;
extern const int exitGameButtonH;
extern const int mapChangeButtonW;
extern const int mapChangeButtonH;

extern int map_index;

extern int bkg_x;
extern int bkg_y;
extern int bkg_w;
extern int bkg_h;

class MenuScene : public Scene {
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() {
		StartGameButton = startGameButton[0];
		ExitGameButton = exitGameButton[0];
		MapButton = mapChangeButton[0];
		GameMap = gameMaps[map_index];


	}
	void on_update() {}
	void on_draw() {
		putimage(0, 0, &GameMap);
		Alpha_putimage(start_button_x, start_button_y, StartGameButton);
		Alpha_putimage(exit_button_x, exit_button_y, ExitGameButton);
		Alpha_putimage(map_button_x, map_button_y, MapButton);
		if (is_start_game) {
			scene_manager.switch_to(SceneManager::SceneType::Game);
			is_start_game = false;
		}
		else if (is_exit_game) {
			running = false;
		}
	}
	void on_input(const ExMessage& msg) {
		if (start_button_x <= msg.x && msg.x <= start_button_x + start_button_w && start_button_y <= msg.y && msg.y <= start_button_y + start_button_h) {
			StartGameButton = startGameButton[1];
			if (msg.message == WM_LBUTTONDOWN) {
				mciSendString(L"play startgame from 0", NULL, 0, NULL);
				StartGameButton = startGameButton[2];
				is_start_game = true;
			}
		}
		else {
			StartGameButton = startGameButton[0];
		}

		if (exit_button_x <= msg.x && msg.x <= exit_button_x + exit_button_w && exit_button_y <= msg.y && msg.y <= exit_button_y + exit_button_h) {
			ExitGameButton = exitGameButton[1];
			if (msg.message == WM_LBUTTONDOWN) {
				mciSendString(L"play exitgame from 0", NULL, 0, NULL);
				Sleep(300);
				ExitGameButton = exitGameButton[2];
				is_exit_game = true;
			}
		}
		else {
			ExitGameButton = exitGameButton[0];
		}

		if (map_button_x <= msg.x && msg.x <= map_button_x + map_button_w && map_button_y <= msg.y && msg.y <= map_button_y + map_button_h) {
			MapButton = mapChangeButton[1];
			if (msg.message == WM_LBUTTONDOWN) {
				mciSendString(L"play exitgame from 0", NULL, 0, NULL);
				map_index = (map_index + 1) % gameMaps.size();
				map_manager.SwitchMap(map_index);

			}
		}
		else {
			MapButton = mapChangeButton[0];
		}

	}

	void on_exit() {
		BeginBatchDraw();
		while (menu_exit_timer--) {
			cleardevice();
			unsigned char alpha = 255 * menu_exit_timer / menu_exit_delay;
			Alpha_putimage(0, 0, GameMap, alpha);

			FlushBatchDraw();
		}
		EndBatchDraw();
		menu_exit_timer = menu_exit_delay;
	}

private:
	int start_button_w = startGameButtonW;
	int start_button_h = startGameButtonH;
	int start_button_x = GameW / 2 - start_button_w / 2;
	int start_button_y = 370 - start_button_h / 2;

	int exit_button_w = exitGameButtonW;
	int exit_button_h = exitGameButtonH;
	int exit_button_x = GameW / 2 - exit_button_w / 2;
	int exit_button_y = 650 - exit_button_h / 2;

	int map_button_w = mapChangeButtonW;
	int map_button_h = mapChangeButtonH;
	int map_button_x = GameW - map_button_w;
	int map_button_y = 530 - map_button_h / 2;


	IMAGE StartGameButton;
	IMAGE ExitGameButton;
	IMAGE MapButton;

	bool is_start_game = false;
	bool is_exit_game = false;



	const int menu_exit_delay = 100;
	int menu_exit_timer = menu_exit_delay;//菜单界面退出动画计时器
};
