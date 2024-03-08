//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_BERTH_H
#define INC_24_HUAWEI_BERTH_H

#include <iostream>
#include "Ship.h"

using namespace std;

class Berth {
public:
    //泊位id
    int id;
    //泊位位置（左上角坐标）
    pair<int, int> pos;
    //运输时间
    int transport_time;
    //装载时间
    int loading_speed;
    //现有货物数量
    int goods;
    //泊位上的船
    Ship &ship;

    //初始化
    Berth(int id, int x, int y, int transport_time, int loading_speed);
    Berth();
};


#endif //INC_24_HUAWEI_BERTH_H
