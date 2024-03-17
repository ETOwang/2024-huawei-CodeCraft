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
    for (auto it = items.begin(); it !=items.end() ; ) {
        if(it->is_locked){
            ++it;
            continue;
        }
        if(!it->isValid(time)){
            it=items.erase(it);
        } else{
            break;
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
static int __dis[SIZE][SIZE];


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
    array<Coord, 4> diff = {Coord{+1, 0}, Coord{-1, 0}, Coord{0, +1}, Coord{0, -1}};
    static priority_queue<pair<int, Coord>> que;

    //更新：唯一tag
    if(!__vis_uniq_tag){
        std::memset(__vis, 0, sizeof(__vis));
    }
    __vis_uniq_tag ++;

    //清空
    while(que.size()) que.pop();

    auto f = [](Coord st, Coord ed){
        return abs(st[0] - ed[0]) + abs(st[1] - ed[1]);
    };
    auto doPush = [&f, &targ](Coord pos){
        __vis[pos[0]][pos[1]] = __vis_uniq_tag;
        que.push(make_pair(-(__dis[pos[0]][pos[1]] + f(pos, targ)), pos));
    };
    __dis[src[0]][src[1]] = 0;
    //Astar
    doPush(src);
    while(que.size() && (__vis[targ[0]][targ[1]] != __vis_uniq_tag)){
        Coord nw = que.top().second;
        que.pop();
        for(auto it:diff){
            Coord nxt = Coord{nw[0] + it[0], nw[1] + it[1]};
            if(!this -> isGround(nxt)) continue;
            if(__vis[nxt[0]][nxt[1]] == __vis_uniq_tag && __dis[nxt[0]][nxt[1]] <= __dis[nw[0]][nw[1]] + 1) continue;
            __dis[nxt[0]][nxt[1]] = __dis[nw[0]][nw[1]] + 1;
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

vector<Coord> Map::getFreeSpace(Coord src, vector<Coord> ban, vector<Coord> exit){
    array<Coord, 4> diff = {Coord{+1, 0}, Coord{-1, 0}, Coord{0, +1}, Coord{0, -1}};
    random_shuffle(diff.begin(), diff.end());
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

vector<int> Map::getDisVector(Coord src, vector<Coord> targ){
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
    __dis[src[0]][src[1]] = 0;
    doPush(src);
    while(que.size()){
        Coord nw = que.front();
        que.pop();
        for(auto it:diff){
            Coord nxt = Coord{nw[0] + it[0], nw[1] + it[1]};
            if(!this -> isGround(nxt)) continue;
            if(__vis[nxt[0]][nxt[1]] == __vis_uniq_tag) continue;
            __dis[nxt[0]][nxt[1]] = __dis[nw[0]][nw[1]] + 1;
            __prev[nxt[0]][nxt[1]] = nw;
            doPush(nxt);
        }
    }
    //回溯
    vector<int> result;
    for(auto it:targ){
        if(__vis[it[0]][it[1]] != __vis_uniq_tag) result.push_back(1000000000);
        else result.push_back(__dis[it[0]][it[1]]);
    }
    return result;
}

void Map::build() {
    for (int i = 0; i <= SIZE*SIZE; ++i) {
        parent[i]=i;
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if(!isGround({i,j})){
                continue;
            }
            if(isGround({i-1,j})){
                unite({i,j},{i-1,j});
            }
            if(isGround({i+1,j})){
                unite({i,j},{i+1,j});
            }
            if(isGround({i,j+1})){
                unite({i,j},{i,j+1});
            }
            if(isGround({i,j-1})){
                unite({i,j},{i,j-1});
            }
        }
    }
}
int Map::transform(Coord x) {
    return x[0]*SIZE+x[1];
}

void Map::unite(Coord x, Coord y) {
    parent[find(transform(x))]= find(transform(y));
}

int Map::find(int x) {
    if(parent[x]== x){
        return x;
    }
    return parent[x]= find(parent[x]);
}

bool Map::isCommunicated(Coord x, Coord y) {
    if(find(transform(x))== find(transform(y))){
        return true;
    }
    return false;
}