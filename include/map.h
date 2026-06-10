#pragma once

#include <graphics.h>
#include <string>
#include "player.h"

extern IMAGE GameMap;

class Map {
public:
    Map() = default;
    virtual ~Map() = default;

    virtual void on_enter() {}
    virtual void on_exit() {}

    virtual void Render() {
        // 子类应实现具体渲染逻辑
    }

    virtual void on_update(player& p) {
        // 子类应实现具体逻辑更新
    }

    std::string GetName() const { return name; }

protected:
    std::string name;
    IMAGE* background = nullptr;

private:
};
