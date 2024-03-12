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
    //note：map里只存储 '.', '*', '#', 'B' 四种字符，机器人所在位置存储为空地 '.'
    char map[SIZE][SIZE];
    //物品列表
    deque<Item> items;
    //添加一个物品
    void addItem(Item item);
    //对当前位置进行检查，清理无用物品
    void updateItem(int time);

    //规划一条路径
    vector<Coord> getRoute(Coord src, Coord targ);
    vector<Coord> getRoute_BFS(Coord src, Coord targ);
    vector<Coord> getRoute_Astar(Coord src, Coord targ);
    vector<Coord> getFreeSpace(Coord src, vector<Coord> ban);


    //一些帮助函数
    //检测是否越界
    bool isOutOfBound(Coord pos);
    //检测是否为地面（空地或泊位）
    bool isGround(Coord pos);
};


#endif //INC_24_HUAWEI_MAP_H
