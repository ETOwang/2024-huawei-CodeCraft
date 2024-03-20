//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_CONTROLLER_H
#define INC_24_HUAWEI_CONTROLLER_H
#define TRANSPORT 500
#define EXPECTED 100
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
    int berth_index;
    Controller(Robot* robots,int robot_num,Map* map,Ship* ships,int ship_num,Berth* berths,int berth_num);
    Controller();
    //主调度函数，为所有目标分配任务（如果需要）
    void dispatch(int time);
    int assignBerth(Robot* robot);
    bool isCollision(Robot* robot1,Robot* robot2);
    bool isSwap(Robot* robot1,Robot* robot2);
    void assignShip();
    bool haveChanceToGo(int berth);
    void judgeTime(int time);
    void moveToBerth();
};


#endif //INC_24_HUAWEI_CONTROLLER_H
