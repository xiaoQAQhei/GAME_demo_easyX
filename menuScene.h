#pragma once
#include "scene.h"
#include"scene_manager.h"

extern SceneManager scene_manager;

extern IMAGE menu_background;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() {}
	void on_update() {}
	void on_draw() {
		putimage(0, 0, &menu_background);
	}
	void on_input(const ExMessage& msg) {
		if (msg.message == WM_KEYUP) {
			scene_manager.switch_to(SceneManager::SceneType::Game);
		}

	}
	void on_exit() {}

private:

};
