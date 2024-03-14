cmake_minimum_required(VERSION 3.23)
project(24_huawei)

set(CMAKE_CXX_STANDARD 14)

add_executable(24_huawei
        main.cpp Map.cpp Map.h Robot.cpp Robot.h Item.cpp Item.h Ship.cpp Ship.h Controller.cpp Controller.h Berth.cpp Berth.h Utils.cpp Utils.h )
