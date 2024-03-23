#ifndef INC_24_HUAWEI_UTILS_H
#define INC_24_HUAWEI_UTILS_H

#include <array>
#include <cstdio>
#include <random>

using namespace std;

using Coord = array<int, 2>;

int operator - (Coord a,Coord b);
static FILE *log_fp = fopen("log.txt","w");

const int log_level = 2;
// 0 = none
// 1 = normal
// 2 = debug

bool shouldLog(int x);

static random_device r_d;
// static mt19937 mt199(r_d());
static mt19937 mt199(1);

#endif //INC_24_HUAWEI_UTILS_H
