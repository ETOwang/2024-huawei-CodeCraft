//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_CONTROLLER_H
#define INC_24_HUAWEI_CONTROLLER_H

#include "Utils.h"
#include "Item.h"
#include "Map.h"
#include "Robot.h"
#include "Ship.h"
class Controller {
public:
    Map* game_map;
    Robot* robots;
    Ship* ships;
    int robot_num;
    int ships_num;
    Controller(Robot* robots,int robot_num,Map* map,Ship* ships,int ship_num);
    Controller();
    //主调度函数，为所有目标分配任务（如果需要）
    void dispatch();
    int assignBerth(Robot* robot);
    int getdis(Robot* robot,Item* item);
    bool collision(Robot* robot1,Robot* robot2);
};


#endif //INC_24_HUAWEI_CONTROLLER_H
