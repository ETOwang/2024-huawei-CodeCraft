#include "Utils.h"


bool shouldLog(int x) {
    return x <= log_level;
}

int operator - (Coord a,Coord b){
    return abs(a[0]-b[0])+ abs(a[1]-b[1]);
}