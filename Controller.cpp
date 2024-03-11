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
    for (int i = 0; i < robot_num; ++i) {
        if (robots[i].task_type == TaskIdle) {
            int berth = assignBerth(&robots[i]);
            Item *best_item = nullptr;
            for (auto &item: game_map->items) {
                if (!item.isValid(time)) {
                    continue;
                }
                if (best_item == nullptr) {
                    best_item = &item;
                } else {
                    if (best_item->value /
                        (getdis(robots[i].pos, best_item->pos) + getdis(best_item->pos, berths[berth].pos)) <
                        item.value /
                        (getdis(robots[i].pos, item.pos) + getdis(item.pos, berths[berth].pos))) {
                        best_item = &item;
                    }
                }
            }
            if (best_item == nullptr) {
                robots[i].berth_pos = berths[berth].pos;
                continue;
            }
            best_item->setInvalid();
            robots[i].task_type = TaskItem;
            robots[i].setRoute(best_item->pos);
            robots[i].route = game_map->getRoute(robots[i].pos, best_item->pos);
            robots[i].berth_pos = berths[berth].pos;
            robots[i].item_pos = best_item->pos;
        } else if (robots[i].task_type == TaskItem) {
            if (robots[i].route.empty()) {
                robots[i].task_type = TaskBerth;
                robots[i].route = game_map->getRoute(robots[i].pos, robots[i].berth_pos);
                for (auto &item: game_map->items) {
                    if (item.pos == robots->item_pos) {
                        item.setInvalid();
                        break;
                    }
                }
            }
        } else {
            if (robots[i].route.empty()) {
                robots[i].task_type = TaskIdle;
                for (int j = 0; j < berth_num; ++j) {
                    if (berths[j].pos == robots[i].berth_pos) {
                        for (auto &item: game_map->items) {
                            if (item.pos == robots[i].item_pos) {
                                berths[j].addItem(&item);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < berth_num; ++i) {
        if (time > 10000) {
            berths[i].ship->force_to_go = true;
        }
        if (berths[i].transport_time + time >= 14000) {
            berths[i].ship->force_to_go = true;
        }
    }
    for (int i = 0; i < robot_num; ++i) {
        for (int j = 0; j < robot_num; ++j) {
            if (i == j) {
                continue;
            }
            if (collision(&robots[i], &robots[j])) {
                auto next1 = robots[i].route.top();
                auto next2 = robots[j].route.top();
                if (next1 == robots[j].pos && next2 == robots[i].pos) {
                    robots[i].route.push(robots[i].pre_pos.top());
                    robots[i].pre_pos.pop();
                    robots[i].route.push(robots[i].pre_pos.top());
                    robots[i].pre_pos.pop();
                    continue;
                }
                if (next1 == next2) {
                    robots[i].route.push(robots[i].pos);
                }

            }
        }
    }
    //船舶需要船的引用
    for (int i = 0; i < ships_num; ++i) {
        if (ships[i].status == 1 && ships[i].target_id == -1) {
            for (auto berth: used) {
                if (berths[berth.first].ship == nullptr || berths[berth.first].ship == &ships[i]) {
                    berths[berth.first].ship = &ships[i];
                    ships[i].target_id = berth.first;
                    break;
                }
            }
        }
    }
}

int Controller::assignBerth(Robot *robot) {
    if (robot->berth_pos[0] != -1) {
        for (int i = 0; i < berth_num; ++i) {
            if (berths[i].pos == robot->berth_pos) {
                return i;
            }
        }
    }
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
    if (robot1->route.empty() || robot2->route.empty()) {
        return false;
    }
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


int Controller::getdis(Coord robot, Coord item) {
    auto route = game_map->getRoute(robot, item);
    if (route.size() == 0) {
        return 1;
    }
    return route.size();
}
