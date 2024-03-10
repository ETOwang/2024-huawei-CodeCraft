//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_BERTH_H
#define INC_24_HUAWEI_BERTH_H

#include <stack>
#include "Utils.h"
#include <vector>
#include "Item.h"
class Berth {
public:
    //泊位id
    int id;
    //泊位位置（左上角坐标）
    Coord pos;
    //运输时间
    int transport_time;
    //装载速度
    int loading_speed;
    //现有货物
    vector<Item*> goods;
    //泊位上的船
    int ship_id;

    //初始化
    Berth(int id, Coord pos, int transport_time, int loading_speed);
    Berth();
};


#endif //INC_24_HUAWEI_BERTH_H
