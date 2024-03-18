//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_ROBOT_H
#define INC_24_HUAWEI_ROBOT_H

#include <iostream>
#include <vector>
#include "Utils.h"
#include "Item.h"

enum RobotTaskType{
    TaskIdle, TaskItem, TaskBerth
};

class Robot {
public:
    //机器人id
    int id;
    //机器人位置
    Coord pos;
    //机器人状态
    int status;

    Item *cur_item;

    //任务类型
    RobotTaskType task_type;
    //TaskItem 任务的目标
    Coord item_pos;
    //TaskBerth 任务的目标
    Coord berth_pos;
    //当前任务路径（包括起始点和结束点）
    vector<Coord> route;
    //初始化
    Robot(int id, Coord pos);
    Robot();
    void move();
};


#endif //INC_24_HUAWEI_ROBOT_H
