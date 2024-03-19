//
// Created by 15461 on 2024/3/8.
//


#include <algorithm>
#include <set>
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
        auto getBestItem = [this, &time](Coord robot_pos, Coord berth_pos) -> Item * {
            Item *best_item = nullptr;
            double best_eval = -1;
            vector<Item *> item_t;
            for (auto &item: game_map->items) {
                if (item.is_locked) {
                    continue;
                }
                if (!game_map->isCommunicated(item.pos, robot_pos)) {
                    continue;
                }
                item_t.push_back(&item);
            }
            vector<Coord> targ;
            for (auto it: item_t) targ.push_back((*it).pos);
            vector<int> dis1 = game_map->getDisVector(robot_pos, targ);
            vector<int> dis2 = game_map->getDisVector(berth_pos, targ);
            if (dis1.empty()) return nullptr;
            for (int j = 0; j < dis1.size(); j++) {
                double eval = item_t[j]->value
                        / pow((double) dis1[j] + dis2[j], 1);
                if (time + dis1[j] + dis2[j] > item_t[j]->time + item_t[j]->time_before_disappear) {
                    continue;
                }
                if (best_eval < eval) {
                    best_item = item_t[j];
                    best_eval = eval;
                }
            }
            return best_item;
        };

        if (robots[i].task_type == TaskIdle) {
            int berth = assignBerth(&robots[i]);
            if (berth == -1) {
                continue;
            }
            robots[i].berth_pos = berths[berth].pos;
            Item *best_item = nullptr;
            // find the best item
            best_item = getBestItem(robots[i].pos, robots[i].berth_pos);

            if (best_item == nullptr || best_item->pos == robots[i].item_pos) {
                continue;
            }
            best_item->lock();
            robots[i].task_type = TaskItem;
            robots[i].route = game_map->getRoute(robots[i].pos, best_item->pos);
            robots[i].item_pos = best_item->pos;
            robots[i].cur_item = best_item;
        } else if (robots[i].task_type == TaskItem) {
            if (robots[i].route.empty()) {
                robots[i].task_type = TaskBerth;
                assignBerth(&robots[i]);
                robots[i].route = game_map->getRoute(robots[i].pos, robots[i].berth_pos);
            } else {
                // re-select item

                /*if (time % 10 == i) {
                    if (robots[i].cur_item == nullptr) continue;
                    Item* new_best_item = getBestItem(robots[i].pos, robots[i].berth_pos);
                    if (new_best_item == nullptr) continue;
                    auto dis_old = game_map->getDisVector(robots[i].cur_item->pos, vector<Coord>{robots[i].berth_pos, robots[i].pos});
                    auto dis_new = game_map->getDisVector(new_best_item->pos,      vector<Coord>{robots[i].berth_pos, robots[i].pos});
                    double eval_old = robots[i].cur_item->value / pow((double) dis_old[0] + dis_old[1], 1);
                    double eval_new = new_best_item->value      / pow((double) dis_new[0] + dis_new[1], 1);
                    if (eval_old + 0.001 < eval_new) {
                        new_best_item -> lock();
                        robots[i].cur_item -> unlock();
                    //  robots[i].cur_item = nullptr; // this line rejects future update
                        robots[i].cur_item = new_best_item;
                        robots[i].route = game_map->getRoute(robots[i].pos, new_best_item->pos);
                        robots[i].item_pos = new_best_item->pos;
                    }
                }*/

            }
        } else {
            if (robots[i].route.empty()) {
                robots[i].task_type = TaskIdle;
                for (int j = 0; j < berth_num; ++j) {
                    if (berths[j].pos == robots[i].berth_pos) {
                        for (auto &item: game_map->items) {
                            if (item.pos == robots[i].item_pos) {
                                berths[j].addItem(&item);
                                item.unlock();
                                item.setInvalid();
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    judgeTime(time);
//  array<int, 10> random_order{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//  random_shuffle(random_order.begin(), random_order.end());
    vector<int> random_order_tmp, deg_tmp;
    queue<int> que_tmp;
    for (int i = 0; i < robot_num; ++i) {
        array<Coord, 4> diff = {Coord{-1, 0}, Coord{1, 0}, Coord{0, -1}, Coord{0, 1}};
        int deg = 0;
        for (auto it: diff) {
            Coord nw = robots[i].pos;
            Coord to = Coord{nw[0] + it[0], nw[1] + it[1]};
            int add = 1;
            if (!game_map->isGround(to)) add = 0;
            for (int j = 0; j < robot_num; ++j) {
                if (robots[j].pos == to) add = 0;
            }
            deg += add;
        }
        deg_tmp.push_back(deg);
        if (deg) {
            que_tmp.push(i);
        }
    }
    while (!que_tmp.empty()) {
        int nwid = que_tmp.front();
        que_tmp.pop();
        random_order_tmp.push_back(nwid);
        array<Coord, 4> diff = {Coord{-1, 0}, Coord{1, 0}, Coord{0, -1}, Coord{0, 1}};
        int deg = 0;
        for (auto it: diff) {
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
                const int retreat_length = 512;
                const int uniq_length = 128;
                set<Coord> ret_pos_temp;
                for (int k = 1; k <= retreat_length; k++) {
                    if (k > robots[now_j].route.size()) break;
                    ret_pos_temp.insert(*(robots[now_j].route.end() - k));
                    if (ret_pos_temp.size() >= uniq_length) break;
                }
                for (auto pos: ret_pos_temp) exit.push_back(pos);

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
    assignShip();
}

int Controller::assignBerth(Robot *robot) {
    return robot->id;
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


void Controller::assignShip() {
    for (int i = 0; i < ships_num; ++i) {
        if (ships[i].status == 1 ) {
            if(ships[i].target_id==-1){
                for (int j = 0; j < berth_num; ++j) {
                    if(berths[j].is_chose){
                        continue;
                    } else{
                        ships[i].target_id=j;
                        berths[j].lock();
                        berths[j].ships.push(&ships[i]);
                        break;
                    }
                }
            } else{
                if(berths[ships[i].target_id].goods.empty()){
                    for (int j = 0; j < berth_num; ++j) {
                        if(!haveChanceToGo(j)||berths[j].goods.empty()||berths[j].is_chose){
                            continue;
                        }
                        if(TRANSPORT+berths[j].goods.size()/berths[j].loading_speed<berths[j].goods.size()*EXPECTED){
                            berths[ships[i].target_id].ships.pop();
                            berths[ships[i].target_id].unlock();
                            ships[i].target_id=j;
                            ships[i].force_to_ship=j;
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool Controller::haveChanceToGo(int berth) {
    if (berths[berth].transport_time + TRANSPORT+game_map->time>15000) {
        return false;
    }
    return true;
}

void Controller::judgeTime(int time) {
    for (int i = 0; i < berth_num; ++i) {
        if (berths[i].ships.empty()) {
            continue;
        }
        if (berths[i].transport_time + time >= 15000) {
            while (!berths[i].ships.empty()) {
                berths[i].ships.front()->force_to_go = true;
                berths[i].ships.pop();
            }
            berths[i].can_be_used= false;
        }
    }
}