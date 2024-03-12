#ifndef INC_24_HUAWEI_UTILS_H
#define INC_24_HUAWEI_UTILS_H

#include <array>
#include <cstdio>

using namespace std;

using Coord = array<int, 2>;

static FILE *log_fp = fopen("log.txt","w");

const int log_level = 2;
// 0 = none
// 1 = normal
// 2 = debug

bool shouldLog(int x);

#endif //INC_24_HUAWEI_UTILS_H
