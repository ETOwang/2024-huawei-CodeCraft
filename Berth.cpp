//
// Created by 15461 on 2024/3/8.
//

#include "Berth.h"

Berth::Berth(int id, Coord pos, int transport_time, int loading_speed){
    this -> id = id;
    this -> pos = pos;
    this -> transport_time = transport_time;
    this -> loading_speed = loading_speed;
}
Berth::Berth(){
    ;
}