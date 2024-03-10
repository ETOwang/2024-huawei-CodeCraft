//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_CONTROLLER_H
#define INC_24_HUAWEI_CONTROLLER_H

#include "Utils.h"
#include "Item.h"
#include "Map.h"
#include "Robot.h"
class Controller {
public:
    Map* game_map;
    Robot* robots;
    //标记是否已为所有机器人和轮船分配任务

    //主调度函数，为所有目标分配任务（如果需要）
    void dispatch();
    int assignBerth(Robot* robot);
    int getdis(Robot* robot,Item* item);
    bool collision(Robot* robot1,Robot* robot2);
};


#endif //INC_24_HUAWEI_CONTROLLER_H
