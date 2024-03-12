//
// Created by 15461 on 2024/3/8.
//

#include <cstring>
#include <algorithm>
#include "Map.h"

bool Map::isOutOfBound(Coord pos){
    if(pos[0] < 0 || pos[0] >= SIZE) return true;
    if(pos[1] < 0 || pos[1] >= SIZE) return true;
    return false;
}
bool Map::isGround(Coord pos){
    if(this -> isOutOfBound(pos)) return false;
    if(this -> map[pos[0]][pos[1]] == '.'
        || this -> map[pos[0]][pos[1]] == 'B'){
        return true;
    }
    return false;
}

//添加一个物品
void Map::addItem(Item item){
    this -> items.push_back(item);
}
//对当前位置进行检查，清理无用物品
void Map::updateItem(int time){
    for (auto item = items.begin(); item !=items.end() ; ) {
        if((*item).isValid(time)){
            break;
        } else{
            if((*item).time>0){
                item=items.erase(item);
            } else{
                item++;
            }
        }
    }
}

//寻路
vector<Coord> Map::getRoute(Coord src, Coord targ){
    return this -> getRoute_BFS(src, targ);
}


//保存上一个位置
static Coord __prev[SIZE][SIZE];
//标记是否bfs过。用唯一tag代替初始化
static int __vis[SIZE][SIZE], __vis_uniq_tag = 0;


vector<Coord> Map::getRoute_BFS(Coord src, Coord targ){
    array<Coord, 4> diff = {Coord{+1, 0}, Coord{-1, 0}, Coord{0, +1}, Coord{0, -1}};
    static queue<Coord> que;

    //更新：唯一tag
    if(!__vis_uniq_tag){
        std::memset(__vis, 0, sizeof(__vis));
    }
    __vis_uniq_tag ++;

    //清空
    while(que.size()) que.pop();


    auto doPush = [](Coord pos){
        __vis[pos[0]][pos[1]] = __vis_uniq_tag;
        que.push(pos);
    };
    //BFS
    doPush(src);
    while(que.size() && (__vis[targ[0]][targ[1]] != __vis_uniq_tag)){
        Coord nw = que.front();
        que.pop();
        for(auto it:diff){
            Coord nxt = Coord{nw[0] + it[0], nw[1] + it[1]};
            if(!this -> isGround(nxt)) continue;
            if(__vis[nxt[0]][nxt[1]] == __vis_uniq_tag) continue;
            __prev[nxt[0]][nxt[1]] = nw;
            doPush(nxt);
        }
    }
    //回溯
    if(__vis[targ[0]][targ[1]] != __vis_uniq_tag) return vector<Coord> {};
    vector<Coord> result;
    for(Coord nw = targ; nw != src; nw = __prev[nw[0]][nw[1]]){
        result.push_back(nw);
    }
    return result;
}

vector<Coord> Map::getRoute_Astar(Coord src, Coord targ){
    // do something.
    return vector<Coord>{};
}

vector<Coord> Map::getFreeSpace(Coord src, vector<Coord> ban, vector<Coord> exit){
    array<Coord, 4> diff = {Coord{+1, 0}, Coord{-1, 0}, Coord{0, +1}, Coord{0, -1}};
    static queue<Coord> que;

    //更新：唯一tag
    if(!__vis_uniq_tag){
        std::memset(__vis, 0, sizeof(__vis));
    }
    __vis_uniq_tag ++;

    //清空
    while(que.size()) que.pop();


    auto doPush = [](Coord pos){
        __vis[pos[0]][pos[1]] = __vis_uniq_tag;
        que.push(pos);
    };
    //BFS
    doPush(src);

    auto isEmptyGround = [this, &ban](Coord pos) -> bool{
        if(!isGround(pos)) return false;
        for(auto it : ban) {
            if(it == pos) return false;
        }
        return true;
    };
    auto isExit = [this, &exit](Coord pos) -> bool{
        if(!isGround(pos)) return false;
        for(auto it : exit) {
            if(it == pos) return false;
        }
        return true;
    };

    Coord ans = src;
    while(que.size()){
        Coord nw = que.front();
        que.pop();

        ans = nw;
        if(isExit(nw)){
            ans = nw;
        //  if(rand() & 3 == 3) break;
            break;
        }
        random_shuffle(diff.begin(), diff.end());
        for(auto it:diff){
            Coord nxt = Coord{nw[0] + it[0], nw[1] + it[1]};
            if(!isEmptyGround(nxt)) continue;
            if(__vis[nxt[0]][nxt[1]] == __vis_uniq_tag) continue;
            __prev[nxt[0]][nxt[1]] = nw;
            doPush(nxt);
        }
    }
    //回溯
    if(ans == src) return vector<Coord> {};
    vector<Coord> result;
    vector<Coord> tmp;
    for(Coord nw = ans; nw != src; nw = __prev[nw[0]][nw[1]]){
        if(nw != ans) tmp.push_back(nw);
    }
    result.push_back(src);
    reverse(tmp.begin(), tmp.end());
    for(auto it : tmp) result.push_back(it);
    result.push_back(ans);
    reverse(tmp.begin(), tmp.end());
    for(auto it : tmp) result.push_back(it);
    return result;
}