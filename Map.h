//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_MAP_H
#define INC_24_HUAWEI_MAP_H

#include <queue>
#include "Utils.h"
#include "Item.h"

const int SIZE = 200;

class Map {
public:
    //当前帧数
    int time;
    //地图
    char map[SIZE][SIZE];
    //物品列表
    queue<Item> items[SIZE][SIZE];
    //添加一个物品
    void addItem(Item item, Coord pos);
    //对当前位置进行检查，清理无用物品
    void updateItem(Coord pos);
};


#endif //INC_24_HUAWEI_MAP_H
