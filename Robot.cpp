//
// Created by 15461 on 2024/3/8.
//

#include "Robot.h"

//初始化
Robot::Robot(int id, Coord pos){
    this -> id = id;
    this -> pos = pos;
}
Robot::Robot(){

}

void Robot::move() {
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

//根据目标设置路径
void Robot::setRoute(Coord target){

}