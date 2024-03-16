//
// Created by 15461 on 2024/3/8.
//

#include <random>
#include <algorithm>
#include <climits>
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
            if(berth==-1){
                continue;
            }
            robots[i].berth_pos = berths[berth].pos;
            Item *best_item = nullptr;
            double best_eval = -1;
            // find the best item
            // get dis...
            vector<Item*> item_t;
            for (auto &item: game_map->items) {
                if (!item.isValid(time)) {
                    continue;
                }
                if (!game_map->isCommunicated(item.pos, robots[i].pos)) {
                    continue;
                }
                item_t.push_back(&item);
            }
            vector<Coord> targ;
            for (auto it:item_t) targ.push_back((*it).pos);
            vector<int> dis = game_map->getDisVector(robots[i].pos, targ);
            if(!dis.size()) continue;
            for(int i = 0; i < dis.size(); i ++) {
                double eval = item_t[i]->value /
                              pow((double) dis[i] ,//+ getdis(item.pos, berths[berth].pos),
                                  1);
                if (best_eval < eval) {
                    best_item = item_t[i];
                    best_eval = eval;
                }
            }
            if (best_item == nullptr||best_item->pos==robots[i].item_pos) {
                continue;
            }
            best_item->setInvalid();
            robots[i].task_type = TaskItem;
            robots[i].route = game_map->getRoute(robots[i].pos, best_item->pos);
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
        if (berths[i].ship == nullptr) {
            continue;
        }
        if (berths[i].transport_time + time >= 15000) {
            berths[i].ship->force_to_go = true;
        }
    }
//  array<int, 10> random_order{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//  random_shuffle(random_order.begin(), random_order.end());
    vector<int> random_order_tmp, deg_tmp;
    queue<int> que_tmp;
    for (int i = 0; i < robot_num; ++i) {
        array<Coord, 4> diff = {Coord{-1, 0}, Coord{1, 0}, Coord{0, -1}, Coord{0,1}};
        int deg = 0;
        for (auto it:diff) {
            Coord nw = robots[i].pos;
            Coord to = Coord{nw[0] + it[0], nw[1] + it[1]};
            int add = 1;
            if (!game_map -> isGround(to)) add = 0;
            for (int j = 0; j < robot_num; ++j) {
                if (robots[j].pos == to) add = 0;
            }
            deg += add;
        }
        deg_tmp.push_back(deg);
        if(deg){
            que_tmp.push(i);
        }
    }
    while(que_tmp.size()){
        int nwid = que_tmp.front();
        que_tmp.pop();
        random_order_tmp.push_back(nwid);
        array<Coord, 4> diff = {Coord{-1, 0}, Coord{1, 0}, Coord{0, -1}, Coord{0,1}};
        int deg = 0;
        for (auto it:diff) {
            Coord nw = robots[nwid].pos;
            Coord to = Coord{nw[0] + it[0], nw[1] + it[1]};
            for (int j = 0; j < robot_num; ++j) {
                if (robots[j].pos == to) {
                    if (!deg_tmp[j]) {
                        que_tmp.push(j);
                    }
                    deg_tmp[j]++;
                }
            }
        }
    }
    for (int i = 0; i < robot_num; ++i) {
        if (!deg_tmp[i]) {
            que_tmp.push(i);
        }
    }
//  fprintf(log_fp, "random_order_tmp.size() == %d\n", random_order_tmp.size());
//  assert(random_order_tmp.size() == 10);
    reverse(random_order_tmp.begin(), random_order_tmp.end());
    array<int, 10> random_order{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < robot_num; ++i) {
        random_order[i] = random_order_tmp[i];
    }

    for (int i = 0; i < robot_num; ++i) {
        int now_i = random_order[i];
        for (int j = 0; j < i; ++j) {
            int now_j = random_order[j];
            if (isCollision(&robots[now_i], &robots[now_j])) {
                robots[now_i].route.push_back(robots[now_i].pos);
                ({
                    if (shouldLog(2)) {
                        fprintf(log_fp, "Frame %d: isCollision(%d, %d)\nRobot %d: wait\n", time, now_i, now_j, now_i);
                    }
                });
                break;
            }
            if (isSwap(&robots[now_i], &robots[now_j])) {
                vector<Coord> ban, exit;
                // 不能撞到已经避过障的机器人
                for (int k = 0; k <= i; k++) {
                    int now_k = random_order[k];
                    ban.push_back(robots[now_k].pos);
                    exit.push_back(robots[now_k].pos);
                }
                // 避开要避让的机器人将要走的路
                const int retreat_length = 40;
                for (int k = 1; k <= retreat_length; k++) {
                    if (k > robots[now_j].route.size()) break;
                    exit.push_back(*(robots[now_j].route.end() - k));
                }
                vector<Coord> result = game_map->getFreeSpace(robots[now_i].pos, ban, exit);
                for (auto it: result) {
                    robots[now_i].route.push_back(it);
                }
                ({
                    if (shouldLog(2)) {
                        fprintf(log_fp, "Frame %d: isSwap(%d, %d)\nRobot %d:\n", time, now_i, now_j, now_i);

                        fprintf(log_fp, "ban = {");
                        for (auto it: ban) fprintf(log_fp, "(%d, %d), ", it[0], it[1]);
                        fprintf(log_fp, "}\n");

                        fprintf(log_fp, "exit = {");
                        for (auto it: exit) fprintf(log_fp, "(%d, %d), ", it[0], it[1]);
                        fprintf(log_fp, "}\n");

                        fprintf(log_fp, "add path = {");
                        for (auto it: result) fprintf(log_fp, "(%d, %d), ", it[0], it[1]);
                        for (auto it: result) fprintf(log_fp, "(%d, %d), ", it[0], it[1]);
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
        if (abs(nw[0] - dest[0]) + abs(nw[1] - dest[1]) >= 2) {
            vector<Coord> path = game_map->getRoute(nw, dest);
            for (auto it: path) robots[i].route.push_back(it);
        }
    }
    //船舶需要船的引用
    for (int i = 0; i < ships_num; ++i) {
        if (ships[i].status == 1 && ships[i].target_id == -1) {
            for (auto berth: used) {
                if (berths[berth.first].ship == nullptr) {
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
    for (auto &item: used) {
        if (item.second<2 && game_map->isCommunicated(berths[item.first].pos, robot->pos)) {
            item.second++;
            return item.first;
        }
    }
    for (auto &item: used) {
        if (game_map->isCommunicated(berths[item.first].pos, robot->pos)) {
            item.second++;
            return item.first;
        }
    }
    for (int i = 0; i < berth_num; ++i) {
        if(!used.count(i)&&game_map->isCommunicated(berths[i].pos,robot->pos)){
            used[i]=1;
            return i;
        }
    }
    return -1;
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

//泊位预分配
void Controller::preAssign() {
    double berthValue[berth_num];
    int berthConnected[berth_num];
    double berthDistance[berth_num];
    //确定泊位与机器人的连通性
    for (int i = 0; i < berth_num; ++i) {
        for (int j = 0; j < robot_num; ++j) {
            if (game_map->isCommunicated(berths[i].pos, robots[j].pos)){
                berthConnected[i]++;
            }
        }
    }
    //确定泊位间距离
    for (int i = 0; i < berth_num; ++i) {
        for (int j = 0; j < berth_num; ++j) {
            berthDistance[i] += sqrt(pow((berths[i].pos[0] - berths[j].pos[0]), 2) + pow((berths[i].pos[1] -berths[j].pos[1]), 2));
        }
    }
    for (int i = 0; i < berth_num; ++i) {
        berthValue[i] = para1 * berthConnected[i] + para2 * berths[i].loading_speed -
                para3 * berths[i].transport_time + para4 * berthDistance[i];
    }
    for (int i = 0; i < 5; ++i) {
        int temp = 0;
        int max = INT_MIN;
        for (int j = 0; j < berth_num; ++j) {
            if (berthValue[j] > max){
                temp = j;
                max = berthValue[j];
            }
        }
        berthValue[temp] = INT_MIN;
        used[temp] = 0;
    }
}
