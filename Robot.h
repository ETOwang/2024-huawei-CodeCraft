//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_ROBOT_H
#define INC_24_HUAWEI_ROBOT_H

#include <iostream>
#include <stack>
#include "Utils.h"

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
    
    //任务类型
    RobotTaskType task_type;
    //TaskItem 任务的目标
    Coord item_pos;
    //TaskBerth 任务的目标
    Coord berth_pos;
    //当前任务路径（包括起始点和结束点）
    stack<Coord> route;
    //初始化
    Robot(int id, Coord pos);
    Robot();
    //根据目标设置路径
    void setRoute(Coord target);
    void move(){
        Coord next_pos = route.top();
        route.pop();
        int cur_pos_x = this->pos[0];
        int cur_pos_y = this->pos[1];
        int next_pos_x = next_pos[0];
        int next_pos_y = next_pos[1];
        //到物品 0:右移；1：左移； 2：上移；3：下移；
        printf("move %d ", this->id);
        if (cur_pos_x < next_pos_x){//右移
            printf("0\n");
        } else if (cur_pos_x > next_pos_x){//左
            printf("1\n");
        } else if (cur_pos_y < next_pos_y){//上
            printf("2\n");
        } else if (cur_pos_y > next_pos_y){//下
            printf("3\n");
        } else {
            return;
        }
        //栈为空则到达目的地
        if (route.empty()){
            if (this->task_type == TaskItem){//到达货物
                printf("get %d\n", this->id);
            } else{
                printf("pull %d\n", this->id);
            }
        }
    }
};


#endif //INC_24_HUAWEI_ROBOT_H
