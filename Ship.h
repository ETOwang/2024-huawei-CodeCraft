//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_SHIP_H
#define INC_24_HUAWEI_SHIP_H

#include "Berth.h"

class Ship {
public:
    //轮船id
    int id;
    //轮船状态
    int status;
    //轮船目标泊位
    Berth &berth;
    //轮船已有货物
    int goods;
    //轮船容积
    int capacity;
    //预计完成当前任务时间
    int time;
    //初始化
    Ship(int id, int status);
    Ship();
    //进行对轮船的操作（如果需要）
    void move(int t);

    //进行移动到某个泊位操作
    void ship(int id);

    //进行售卖操作
    void go();
};


#endif //INC_24_HUAWEI_SHIP_H
