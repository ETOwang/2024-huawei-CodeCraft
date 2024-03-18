//
// Created by 15461 on 2024/3/8.
//

#include "Item.h"

Item::Item(int time, Coord pos, int value){
    this -> time = time;
    this -> pos = pos;
    this -> value = value;
    this->is_locked= false;
}

bool Item::isValid(int time){
    if(time - this -> time > time_before_disappear) return false;
    return true;
}

void Item::setInvalid(){
    this -> time = -time_before_disappear;
}

void Item::lock() {
    is_locked= true;
}

void Item::unlock() {
    is_locked= false;
}

