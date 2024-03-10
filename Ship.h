//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_SHIP_H
#define INC_24_HUAWEI_SHIP_H

#include "Utils.h"

class Ship {
public:
    //轮船id
    int id;
    //轮船状态
    int status;
    //轮船目标泊位
    int target_id;

    //轮船已有货物
    int item_count;
    //轮船容积
    int capacity;

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
