#pragma once
#include <graphics.h>
#include <string>
#include <memory>
#include"map1.h"
#include"map2.h"
#include"map.h"

extern vector<IMAGE> gameMaps;

extern Map* map1;
extern Map* map2;

extern int map_index;

class MapManager {
public:
    MapManager() = default;
    ~MapManager() = default;

    void set_current_map(Map* map) {
        currentMap = map;
        currentMap->on_enter();
    }

    void SwitchMap(int map_index) {
        currentMap->on_exit();
        switch (map_index) {
        case 0:
            currentMap = map1;
            break;
        case 1:
            currentMap = map2;
            break;
        }
        GameMap = gameMaps[map_index];
        currentMap->on_enter();
    }

    void RenderCurrentMap() {
        currentMap->Render();
    }

    void on_update(player& p) {
        currentMap->on_update(p);
    }

    Map* GetCurrentMap() const {
        return currentMap;
    }

private:
    Map* currentMap;

};