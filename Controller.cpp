//
// Created by 15461 on 2024/3/8.
//

#include "Controller.h"
#include <random>

Controller::Controller() {

}

Controller::Controller(Robot *robots, int robot_num, Map *map, Ship *ships, int ship_num, Berth *berths,
                       int berth_num) {
    this->robots = robots;
    this->robot_num = robot_num;
    this->game_map = map;
    this->ships = ships;
    this->ships_num = ship_num;
    this->berths = berths;
    this->berth_num = berth_num;
}

void Controller::dispatch(int time) {
    game_map->updateItem(time);
    for (int i = 0; i < robot_num; ++i) {
        if (robots[i].task_type == TaskIdle) {
            int berth = assignBerth(&robots[i]);
            Item *best_item = nullptr;
            for (auto &item: game_map->items) {
                if (best_item == nullptr) {
                    best_item = &item;
                } else {
                    if (best_item->value /
                         (getdis(robots[i].pos, best_item->pos) + getdis(best_item->pos, berths[berth].pos)) <
                         item.value/
                          (getdis(robots[i].pos, item.pos) + getdis(item.pos, berths[berth].pos)) ){
                      best_item=&item;
                    }
                }
            }
            if(best_item == nullptr) continue;
            best_item->setInvalid();
            robots[i].task_type=TaskItem;
            robots[i].route=game_map->getRoute(robots[i].pos,best_item->pos);
            robots[i].berth_pos=berths[berth].pos;
            robots[i].item_pos=best_item->pos;
        } else if(robots[i].task_type==TaskItem){
            if(robots[i].route.empty()){
                robots[i].task_type=TaskBerth;
                robots[i].route=game_map->getRoute(robots[i].pos,robots[i].berth_pos);
            }
        } else{
            if(robots[i].route.empty()){
                robots[i].task_type=TaskIdle;
                for (int j = 0; j < berth_num; ++j) {
                    if(berths[j].pos==robots[i].berth_pos){
                        for (auto& item:game_map->items) {
                           if(item.pos==robots[i].item_pos){
                               berths[j].addItem(&item);
                               break;
                           }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < robot_num; ++i) {
        for (int j = 0; j < robot_num; ++j) {
            if (collision(&robots[i], &robots[j])){
                Controller::deal_collision();
            }
        }
    }
    //船舶需要船的引用
    for (int i = 0; i < ships_num; ++i) {
        if(ships[i].status==1&&ships[i].target_id==-1&&!ships[i].is_on_way){
            ships[i].target_id=used.begin()->first;
            berths[used.begin()->first].ship=&ships[i];
        }
    }
}

int Controller::assignBerth(Robot *robot) {
    if (used.size() == 5) {
        for (auto &item: used) {
            if (item.second == 1) {
                item.second++;
                return item.first;
            }
        }
    }
    std::random_device rd;  // 随机设备，用于获取种子
    std::mt19937 gen(rd());  // 随机数引擎，使用Mersenne Twister算法
    std::uniform_int_distribution<int> dist(0, 9);  // 均匀分布
    int random = dist(gen);
    while (used.count(random) && used[random] == 2) {
        random = dist(gen);
    }
    if (used.count(random)) {
        used[random]++;
    } else {
        used[random] = 1;
    }
    return random;
}

bool Controller::collision(Robot *robot1, Robot *robot2) {
    auto next1 = robot1->route.top();
    auto next2 = robot2->route.top();
    if (next1 == next2) {
        return true;
    }
    if (next1 == robot2->pos && next2 == robot1->pos) {
        return true;
    }
    return false;
}

void deal_collision(){
    //TODO;
}

int Controller::getdis(Coord robot, Coord item) {
    auto route = game_map->getRoute(robot, item);
    return route.size();
}
