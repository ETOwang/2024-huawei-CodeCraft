#include <iostream>
#include "Robot.h"
#include "Controller.h"
using namespace std;

const int robot_num = 10;
const int ship_num=10;
Robot robots[robot_num];
Ship  ships[ship_num];
Controller controller;
//初始化函数，读入初始化信息并创建相应对象
void Init()
{

}
//每一帧读入，并更新相应信息，返回当前帧信息
int Input()
{
}

void finish(){
    printf("OK\n");
    fflush(stdout);
}
/*
 * 优化方向：
 * 1.交互快结束时让港口轮船立刻出发，避免浪费
 * 2.对机器人交叉路径进行碰撞处理
 * 3.尽可能利用轮船运力
 */
int main()
{
    //流程：输入，调度，移动
    Init();
    while (true){
        int frame=Input();
        controller.dispatch();
        for(int i = 0; i < robot_num; i ++)
            robots[i].move(frame);
        for (int i = 0; i < ship_num; ++i) {
            ships[i].move(frame);
        }
        finish();
    }
}
