#include <iostream>
#include "Robot.h"
#include "Ship.h"
#include "Berth.h"
#include "Map.h"
#include "Controller.h"
#include "Ship.h"
using namespace std;
const int size=200;
const int robot_num = 10;
const int ship_num = 10;
Robot robots[robot_num];
Ship ships[ship_num];
Controller controller;
Map game_map;
Berth berths[ship_num];
//初始化函数，读入初始化信息并创建相应对象
void Init() {
    int index=0;
    for (int i = 0; i < size; ++i) {
        char c;
        for (int j = 0; j < size-1; ++j) {
            scanf("%c",&c);
            if(c=='A'){
               robots[index]=Robot(index,{i,j});
               index++;
            }
            game_map.map[i][j]=c;
        }
        scanf("%c\n",&c);
        game_map.map[i][size-1]=c;
    }
    for (int i = 0; i < ship_num; ++i) {
        int id, x, y, time, velocity;
        scanf("%d%d%d%d%d",&id,&x,&y,&time,&velocity);
        berths[i]= Berth(id,{x,y},time,velocity);
    }
    int cap;
    scanf("%d",&cap);
    for (int i = 0; i < ship_num; ++i) {
        ships[i].capacity=cap;
    }
    string temp;
    getline(cin,temp);
}
//每一帧读入，并更新相应信息，返回当前帧信息
int Input()
{
    int frame, temp_money;
    int item_add;
    scanf("%d%d\n", &frame, &temp_money);
    scanf("%d\n", &item_add);
    for (int i = 0; i < item_add; ++i) {
        Coord pos;
        int value;
        scanf("%d%d%d\n", &pos[0], &pos[1], &value);
        Item item = Item(pos, value);
        game_map.addItem(item, pos);
    }
    //输入机器人状态
    for (int i = 0; i < 10; ++i) {
        int item_carry, state;
        scanf("%d%d%d%d\n", &item_carry, &robots[i].pos[0], &robots[i].pos[1], &state);
        robots[i].status = state;
    }
    //输入轮船状态
    for(int i = 0; i < 5; i++){
        scanf("%d%d\n", &ships[i].status, &ships[i].target_id);
    }
    return frame;
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
        /*
        for(int i = 0; i < robot_num; i ++)
            robots[i].move(frame);
        for (int i = 0; i < ship_num; ++i) {
            ships[i].move(frame);
        }
        */
        finish();
    }
}
