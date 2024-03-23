//
// Created by 15461 on 2024/3/8.
//

#include <ctime>
#include "Berth.h"

Berth::Berth(int id, Coord pos, int transport_time, int loading_speed){
    this -> id = id;
    this -> pos = pos;
    this -> transport_time = transport_time;
    this -> loading_speed = loading_speed;
}
Berth::Berth(){
}

void Berth::addItem(Item *item) {
    this->goods.push(item);
}

void Berth::update() {
    if(ships.empty()){
        return;
    }
    if(ships.front()->status!=1){
        return;
    }
    for (int i = 0; i < this->loading_speed; ++i) {
        if(!goods.empty()&&ships.front()->item_count < ships.front()->capacity){
            goods.pop();
            ships.front()->item_count++;
        } else{
            break;
        }
    }
    if(ships.front()->item_count==ships.front()->capacity){
        ships.pop();
    }
}

bool operator<(const Berth& a, const Berth& b) {
    return a.transport_time<b.transport_time;
}

Coord Berth::getPos() {
//  srand(time(0));  // 设置种子为当前时间
    int randomx = rand() % 4;// 生成0到99之间的随机数
    int randomy= rand()%4;
    return {pos[0]+randomx,pos[1]+randomy};
}