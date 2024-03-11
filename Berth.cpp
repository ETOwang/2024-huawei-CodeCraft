//
// Created by 15461 on 2024/3/8.
//

#include "Berth.h"

Berth::Berth(int id, Coord pos, int transport_time, int loading_speed){
    this -> id = id;
    this -> pos = pos;
    this -> transport_time = transport_time;
    this -> loading_speed = loading_speed;
    this->ship= nullptr;
}
Berth::Berth(){
    ship= nullptr;
}

void Berth::addItem(Item *item) {
    this->goods.push(item);
}

void Berth::update() {
    if(ship== nullptr){
        return;
    }
    for (int i = 0; i < this->loading_speed; ++i) {
        if(!goods.empty()&&ship->item_count<ship->capacity){
            goods.pop();
            ship->item_count++;
        } else{
            break;
        }
    }
    if(ship->item_count==ship->capacity){
        ship= nullptr;
    }
}