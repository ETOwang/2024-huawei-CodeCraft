//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_ITEM_H
#define INC_24_HUAWEI_ITEM_H

#include "Utils.h"

class Item {
public:
    bool is_locked;
    int time_before_disappear = 1000;
    //物品的生成时间
    int time;
    //物品位置
    Coord pos;
    //物品直接价值
    int value;

    //初始化函数
    Item(int time, Coord pos, int value);

    //判断是否存在
    bool isValid(int time);
    //强制删除
    void setInvalid();
    void lock();
    void unlock();
};


#endif //INC_24_HUAWEI_ITEM_H
