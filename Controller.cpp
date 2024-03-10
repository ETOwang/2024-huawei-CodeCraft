//
// Created by 15461 on 2024/3/8.
//

#include "Controller.h"

Controller::Controller() {

}

Controller::Controller(Robot *robots, int robot_num, Map *map, Ship *ships,int ship_num) {
    this->robots = robots;
    this->robot_num = robot_num;
    this->game_map = map;
    this->ships=ships;
    this->ships_num=ship_num;
}

void Controller::dispatch() {

}

int Controller::assignBerth(Robot *robot) {

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

int Controller::getdis(Robot *robot, Item *item) {
    auto route = game_map->getRoute(robot->pos, item->pos);
    return route.size();
}