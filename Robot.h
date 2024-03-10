//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_ROBOT_H
#define INC_24_HUAWEI_ROBOT_H

#include <iostream>
#include <stack>
#include "Utils.h"
#include "Item.h"
#include "Berth.h"


using namespace std;

class Robot {
public:
    //机器人位置
    pair<int, int> pos;
    //机器人id
    int id;
    //机器人状态
    int status;
    //目标物品
    Item &tar_item;
    //目标泊位
    Berth &tar_berth;
    //预期出发时间
    int time;
    //当前任务路径（包括起始点和结束点）
    stack<pair<int, int>> route;

    //初始化
    Robot(int id, int startX, int startY);
    Robot();
    //根据目标获取路径
    stack<pair<int, int>> getRoute(pair<int, int> start);

    //进行移动操作（如果需要）
    void move(int time);

    //执行向上操作
    void up();

    //执行向下操作
    void down();

    //执行向左操作
    void left();

    //执行向右操作
    void right();
};


#endif //INC_24_HUAWEI_ROBOT_H
