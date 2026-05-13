#pragma once
#include"scene.h"
#include"scene_manager.h"


extern IMAGE game_background;
extern bool gamerunning;
extern bool gameover;
extern bool gamefinished;


class GameScene :public Scene {
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() {
		gamerunning = true;
	}
	void on_update() {}
	void on_draw() {
		putimage(0, 0, &game_background);
	}
	void on_input(const ExMessage& msg) {
		if (gameover&&msg.message==WM_KEYUP) {
			scene_manager.switch_to(SceneManager::SceneType::Menu);
		}
	}
	void on_exit() {
		gamefinished = true;
		gameover = false;
		gamerunning = false;
	}

private:

};