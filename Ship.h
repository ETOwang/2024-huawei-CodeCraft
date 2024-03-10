//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_SHIP_H
#define INC_24_HUAWEI_SHIP_H
#include <iostream>
using namespace std;
class Ship {
public:
    //时间不够，强制出发
    bool force_to_go;
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
    //判断是否在路上
    bool is_on_way;
    //初始化
    Ship(int id, int status);
    Ship();

    //进行对轮船的操作（如果需要）
    void move();

    //进行移动到某个泊位操作
    void ship(int id);

    //进行售卖操作
    void go();

    //进行卸货行为
    void clear();
};


#endif //INC_24_HUAWEI_SHIP_H
