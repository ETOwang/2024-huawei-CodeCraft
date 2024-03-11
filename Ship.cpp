//
// Created by 15461 on 2024/3/8.
//

#include "Ship.h"


Ship::Ship(int id, int status){
    this -> id = id;
    this -> status = status;
    this->is_on_way= false;
    force_to_go= false;
}
Ship::Ship(){
    this->is_on_way= false;
    force_to_go= false;
}
void Ship::move() {
    if(status!=1){
        return;
    }
    if(!is_on_way&&force_to_go){
        go();
        is_on_way= true;
    }
   if(item_count==capacity){
       go();
       clear();
       is_on_way= true;
   } else if(!is_on_way&&target_id!=-1){
       ship(target_id);
       is_on_way= true;
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