//
// Created by 15461 on 2024/3/8.
//

#include "Ship.h"


Ship::Ship(int id, int status){
    this -> id = id;
    this -> status = status;
}

Ship::Ship(){
    ;
}

void Ship::move(int t) {
    if(status!=1){
        return;
    }
   if(item_count==capacity){
       go();
   } else if(t>time){
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