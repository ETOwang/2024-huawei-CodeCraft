//
// Created by 15461 on 2024/3/8.
//

#include <random>
#include <algorithm>

#include "Controller.h"

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
            double best_eval = -1;
            for (auto &item: game_map->items) {
                if (!item.isValid(time)) {
                    continue;
                }
                if (best_item == nullptr) {
                    best_item = &item;
                } else {
                    double eval = item.value / pow((double)getdis(robots[i].pos, item.pos) + getdis(item.pos, berths[berth].pos), 2);
                    if (best_eval < eval) {
                        best_item = &item;
                        best_eval = eval;
                    }
                }
            }
            if (best_item == nullptr) {
                robots[i].berth_pos = berths[berth].pos;
                continue;
            }
            best_item->setInvalid();
            robots[i].task_type = TaskItem;
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
        if(berths[i].ship== nullptr){
            continue;
        }
        if (berths[i].transport_time + time >= 15000) {
            berths[i].ship->force_to_go = true;
        }
    }
    array<int, 10> random_order{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    random_shuffle(random_order.begin(), random_order.end());
    for (int i = 0; i < robot_num; ++i) {
        int now_i = random_order[i];
        for (int j = 0; j < i; ++j) {
            int now_j = random_order[j];
            if (isCollision(&robots[now_i], &robots[now_j])) {
                robots[now_i].route.push_back(robots[now_i].pos);
                ({
                    if(shouldLog(2)){
                        fprintf(log_fp, "Frame %d: isCollision(%d, %d)\nRobot %d: wait\n", time, now_i, now_j, now_i);
                    }
                });
                break;
            }
            if (isSwap(&robots[now_i], &robots[now_j])) {
                /*
                auto next1 = robots[i].route.top();
                auto next2 = robots[j].route.top();
                if (next1 == robots[j].pos && next2 == robots[i].pos) {
                    robots[i].route.push(robots[i].pre_pos.top());
                    robots[i].pre_pos.pop();
                    if(robots[i].pre_pos.empty()){
                        continue;
                    }
                    robots[i].route.push(robots[i].pre_pos.top());
                    robots[i].pre_pos.pop();
                    continue;
                }
                if (next1 == next2) {
                    robots[i].route.push(robots[i].pos);
                }*/
                vector<Coord> ban, exit;
                // 不能撞到已经避过障的机器人
                for(int k = 0; k <= i; k++) {
                    int now_k = random_order[k];
                    ban.push_back(robots[now_k].pos);
                    exit.push_back(robots[now_k].pos);
                }
                // 避开要避让的机器人将要走的路
                const int retreat_length = 30;
                for(int k = 1; k <= retreat_length; k ++) {
                    if(k > robots[now_j].route.size()) break;
                    exit.push_back(*(robots[now_j].route.end() - k));
                }
                vector<Coord> result = game_map -> getFreeSpace(robots[now_i].pos, ban, exit);
                for(auto it : result) {
                    robots[now_i].route.push_back(it);
                }
                ({
                    if(shouldLog(2)){
                        fprintf(log_fp, "Frame %d: isSwap(%d, %d)\nRobot %d:\n", time, now_i, now_j, now_i);

                        fprintf(log_fp, "ban = {");
                        for(auto it : ban) fprintf(log_fp, "(%d, %d), ", it[0], it[1]);
                        fprintf(log_fp, "}\n");

                        fprintf(log_fp, "exit = {");
                        for(auto it : exit) fprintf(log_fp, "(%d, %d), ", it[0], it[1]);
                        fprintf(log_fp, "}\n");

                        fprintf(log_fp, "add path = {");
                        for(auto it : result) fprintf(log_fp, "(%d, %d), ", it[0], it[1]);
                        for(auto it : result) fprintf(log_fp, "(%d, %d), ", it[0], it[1]);
                        fprintf(log_fp, "}\n");
                    }
                });
                // 尝试等一下来避免卡死
            //  robots[now_j].route.push_back(robots[now_j].pos);
                break;
            }
        }
    }
    for (int i = 0; i < robot_num; ++i) {
        Coord nw = robots[i].pos;
        Coord dest = robots[i].route.empty() ? nw : *--robots[i].route.end();
        if(abs(nw[0] - dest[0]) + abs(nw[1] - dest[1]) >= 2) {
            vector<Coord> path = game_map -> getRoute(nw, dest);
            for(auto it : path) robots[i].route.push_back(it);
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

int Controller::getdis(Coord robot, Coord item) {
    auto route = game_map->getRoute(robot, item);
    if (route.size() == 0) {
        return 1;
    }
    return route.size();
}

bool Controller::isCollision(Robot *robot1, Robot *robot2) {
    auto next1 = robot1->route.empty() ? robot1->pos : *--robot1->route.end();
    auto next2 = robot2->route.empty() ? robot2->pos : *--robot2->route.end();
    if (next1 == next2 && (robot1->pos != next1) && (robot2->pos != next2)) {
        return true;
    }
    return false;
}

bool Controller::isSwap(Robot *robot1, Robot *robot2) {
    auto next1 = robot1->route.empty() ? robot1->pos : *--robot1->route.end();
    auto next2 = robot2->route.empty() ? robot2->pos : *--robot2->route.end();
    if (next1 == robot2->pos && next2 == robot1->pos) {
        return true;
    }
    if (next1 == next2) {
        return true;
    }
    return false;
}

