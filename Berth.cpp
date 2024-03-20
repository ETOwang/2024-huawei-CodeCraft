//
// Created by 15461 on 2024/3/8.
//

#include "Berth.h"

Berth::Berth(int id, Coord pos, int transport_time, int loading_speed){
    this -> id = id;
    this -> pos = pos;
    this -> transport_time = transport_time;
    this -> loading_speed = loading_speed;
    this->can_be_used= true;
    this->is_chose= false;
}
Berth::Berth(){
    this->can_be_used= true;
    this->is_chose= false;
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
    if(ships.empty()){
        unlock();
    }
}

bool operator<(const Berth& a, const Berth& b) {
    return a.transport_time<b.transport_time;
}

void Berth::lock() {
    is_chose= true;
}

void Berth::unlock() {
    is_chose= false;
}