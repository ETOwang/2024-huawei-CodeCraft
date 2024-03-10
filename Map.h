//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_MAP_H
#define INC_24_HUAWEI_MAP_H

#include <queue>
#include <stack>
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
    void updateItem(Coord pos, int time);

    //规划一条路径
    stack<Coord> getRoute(Coord src, Coord targ);
    stack<Coord> getRoute_BFS(Coord src, Coord targ);
    stack<Coord> getRoute_Astar(Coord src, Coord targ);


    //一些帮助函数
    //检测是否越界
    bool isOutOfBound(Coord pos);
    //检测是否为地面（空地或泊位）
    bool isGround(Coord pos);
};


#endif //INC_24_HUAWEI_MAP_H
