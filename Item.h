//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_ITEM_H
#define INC_24_HUAWEI_ITEM_H

#include "Utils.h"

class Item {
public:
    //物品位置
    Coord pos;
    //物品直接价值
    int value;

    //初始化函数
    Item(Coord pos, int value);
};


#endif //INC_24_HUAWEI_ITEM_H
