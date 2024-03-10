//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_CONTROLLER_H
#define INC_24_HUAWEI_CONTROLLER_H

#include "Utils.h"

class Controller {
public:
    //标记是否已为所有机器人和轮船分配任务
    bool flag;

    //主调度函数，为所有目标分配任务（如果需要）
    void dispatch();
};


#endif //INC_24_HUAWEI_CONTROLLER_H
