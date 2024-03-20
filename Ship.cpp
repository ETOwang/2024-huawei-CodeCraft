//
// Created by 15461 on 2024/3/8.
//

#include "Ship.h"


Ship::Ship(){
    force_to_go= false;
    canShip= true;
    canGo= false;
    force_to_ship=-1;
}
void Ship::move() {
    if(status != 1){
        return;
    }
    if (flag){
        ship(target_id);
        go();
        clear();
    }
    if(force_to_ship!=-1){
        ship(target_id);
        force_to_ship=-1;
        return;
    }
    if(canGo&&force_to_go){
        go();
        clear();
        canShip= true;
        canGo= false;
        return;
    }
   if(item_count==capacity&&canGo){
       go();
       clear();
       canShip= true;
       canGo= false;
   } else if(canShip&&target_id!=-1){
       ship(target_id);
       canShip= false;
       canGo= true;
   }
}

void Ship::go() {
    printf("go %d\n", this->id);
}

void Ship::ship(int id) {
    printf("ship %d %d\n", this->id,id);
}

void Ship::clear() {
    item_count=0;
}