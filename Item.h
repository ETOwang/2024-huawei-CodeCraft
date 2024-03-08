//
// Created by 15461 on 2024/3/8.
//

#ifndef INC_24_HUAWEI_ITEM_H
#define INC_24_HUAWEI_ITEM_H

#include <iostream>

using namespace std;

class Item {
public:
    //物品位置
    pair<int, int> pos;
    //物品直接价值
    int value;

    //初始化函数
    Item(int x, int y, int value);

    //价值估算函数（综合考虑）
    int getValue();
};


#endif //INC_24_HUAWEI_ITEM_H
