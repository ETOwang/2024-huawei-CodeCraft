//
// Created by 15461 on 2024/3/8.
//

#include <cstring>
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
    while (!items.empty()){
        if(!items.front().isValid(time)){
            items.pop_front();
        } else{
            break;
        }
    }
}



//寻路
stack<Coord> Map::getRoute(Coord src, Coord targ){
    return this -> getRoute_BFS(src, targ);
}

stack<Coord> Map::getRoute_BFS(Coord src, Coord targ){
    const static Coord diff[4] = {Coord{+1, 0}, Coord{-1, 0}, Coord{0, +1}, Coord{0, -1}};
    //保存上一个位置
    static Coord prev[SIZE][SIZE];
    //标记是否bfs过。用唯一tag代替初始化
    static int vis[SIZE][SIZE], vis_uniq_tag = 0;
    static queue<Coord> que;

    //更新：唯一tag
    if(!vis_uniq_tag){
        std::memset(vis, 0, sizeof(vis));
    }
    vis_uniq_tag ++;

    //清空
    while(que.size()) que.pop();


    auto doPush = [](Coord pos){
        vis[pos[0]][pos[1]] = vis_uniq_tag;
        que.push(pos);
    };
    //BFS
    doPush(src);
    while(que.size() && (vis[targ[0]][targ[1]] != vis_uniq_tag)){
        Coord nw = que.front();
        que.pop();
        for(auto it:diff){
            Coord nxt = Coord{nw[0] + it[0], nw[1] + it[1]};
            if(!this -> isGround(nxt)) continue;
            if(vis[nxt[0]][nxt[1]] == vis_uniq_tag) continue;
            prev[nxt[0]][nxt[1]] = nw;
            doPush(nxt);
        }
    }
    //回溯
    if(vis[targ[0]][targ[1]] != vis_uniq_tag) return stack<Coord> {};
    stack<Coord> result;
    for(Coord nw = targ; nw != src; nw = prev[nw[0]][nw[1]]){
        result.push(nw);
    }
    return result;
}

stack<Coord> Map::getRoute_Astar(Coord src, Coord targ){
    // do something.
    return stack<Coord>{};
}

