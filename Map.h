//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_MAP_H
#define INC_24_HUAWEI_MAP_H

#include <vector>
#include <stack>
#include "Robot.h"
#include "Berth.h"
#include "Ship.h"

const int SIZE = 200;
using namespace std;

class Map {
public:
    //当前帧数
    int time;
    //机器人数量
    int robot_num;
    //泊位数量
    int berth_num;
    //轮船数量
    int ship_num;
    //机器人列表
    Robot *robots;
    //轮船列表
    Ship *ships;
    //泊位列表
    Berth *berths;
    //地图
    char map[SIZE][SIZE];
    //物品列表
    stack<Item *> items;

    //根据时间更新现存物品,泊位和轮船状态
    void update();
};


#endif //INC_24_HUAWEI_MAP_H
