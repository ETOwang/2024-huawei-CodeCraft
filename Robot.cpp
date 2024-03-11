//
// Created by 15461 on 2024/3/8.
//

#include "Robot.h"

//初始化
Robot::Robot(int id, Coord pos) {
    this->id = id;
    this->pos = pos;
    this->berth_pos = {-1, -1};
}

Robot::Robot() {
    this->berth_pos = {-1, -1};
}

void Robot::move() {
    if (route.empty()) {
        return;
    }
    Coord next_pos = route.top();
    int cur_pos_x = this->pos[0];
    int cur_pos_y = this->pos[1];
    int next_pos_x = next_pos[0];
    int next_pos_y = next_pos[1];
    if (abs(cur_pos_x - next_pos_x) + abs(cur_pos_y - next_pos_y) >= 2) {
        route.push(pre_pos.top());
        pre_pos.pop();
        return;
    }
    route.pop();
    // 0:右移；1左移； 2上移；3下移；
    pre_pos.push(next_pos);
    if (cur_pos_x < next_pos_x) {//下移
        printf("move %d 3\n", this->id);
    } else if (cur_pos_x > next_pos_x) {//上
        printf("move %d 2\n", this->id);
    } else if (cur_pos_y < next_pos_y) {//右
        printf("move %d 0\n", this->id);
    } else if (cur_pos_y > next_pos_y) {//左
        printf("move %d 1\n", this->id);
    } else {
        return;
    }
    //栈为空则到达目的地
    if (route.empty()) {
        if (this->task_type == TaskItem) {//到达货物
            printf("get %d\n", this->id);
        } else if (this->task_type == TaskBerth) {
            printf("pull %d\n", this->id);
            this->task_type = TaskIdle;
        }
    }
}

//根据目标设置路径
void Robot::setRoute(Coord target) {
    while (!pre_pos.empty()) {
        pre_pos.pop();
    }
    pre_pos.push(pos);
}