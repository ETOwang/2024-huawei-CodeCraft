//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_CONTROLLER_H
#define INC_24_HUAWEI_CONTROLLER_H
#define para1 0.1
#define para2 0.2
#define para3 100
#define para4 150
#define GAP 200
#include <map>
#include "Utils.h"
#include "Item.h"
#include "Map.h"
#include "Robot.h"
#include "Ship.h"
#include "Berth.h"
class Controller {
public:
    Map* game_map;
    Robot* robots;
    Ship* ships;
    Berth* berths;
    int robot_num;
    int ships_num;
    int berth_num;
    map<int,int> used;
    Controller(Robot* robots,int robot_num,Map* map,Ship* ships,int ship_num,Berth* berths,int berth_num);
    Controller();
    //主调度函数，为所有目标分配任务（如果需要）
    void dispatch(int time);
    int assignBerth(Robot* robot);
    bool isCollision(Robot* robot1,Robot* robot2);
    bool isSwap(Robot* robot1,Robot* robot2);
    void preAssign();
    void assignShip();
    bool haveChanceToGo(int berth);
};


#endif //INC_24_HUAWEI_CONTROLLER_H
