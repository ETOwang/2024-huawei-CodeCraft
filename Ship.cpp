//
// Created by 15461 on 2024/3/8.
//

#include "Ship.h"


Ship::Ship(int id, int status){
    this -> id = id;
    this -> status = status;
    force_to_go= false;
}
Ship::Ship(){
    force_to_go= false;
}
void Ship::move() {
    if(status!=1){
        return;
    }
    if(force_to_go){
        go();
    }
   if(item_count==capacity){
       go();
       clear();
   } else if(target_id!=-1){
       ship(target_id);
   }
}

void Ship::go() {
    printf("go %d\n", this->id);
}

void Ship::ship(int id) {
    printf("ship %d %d\n", this->id,id);
}

void Ship::clear() {
    item_count==0;
}