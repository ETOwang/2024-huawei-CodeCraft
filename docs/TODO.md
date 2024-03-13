## 输入：

读入输入

更新状态

## 规划：

更新物品

机器人寻找目标

机器人寻路

## 其他的处理：

处理碰撞等（可暂时不管）

## 输出：

走

```cpp
for(frames){
    Input();
    Update(); --equals-> {
        for(new goods){
            addItem(new goods);
        }
    }
    Controller.Dispatch(); --equals-> {
        for(robots){
            if(robot.task_type == TaskIdle){
                berth = assignBerth(robot);
                //找物品
                best_item = {};
                for(robot.candidate){
                    if(eval(dis = getdis(robot, candidate) + getdis(candidate, berth), value = candidate.value) is good enough) {
                        best_item = candidate;
                    }
                }
                robot.task_type == TaskItem
                robot.route = getRoute(...);
            } else if(robot.task_type == TaskItem){
                // TBD. [optional]
                // eval(candidate) to get better items
            } else if(robot.task_type == TaskBerth){
                // just walk
            }

        }
    }
    for(robots){
        if(collides(robot.route, robot2.route)) {
            // TBD. [optional]
            // either stop and wait
            // or turn back
            // > advanced: bfs to nearest open space
        }
        robot.Output(); --equals-> {
            out(robot.route.top());
            robot.route.pop();
        }
    }
    for(ships){
        ship.Output();
    }
}
```




---


问题：

+ ~~到某个时间点地图上所有活动均停止，推测出现了死循环~~\
  03/12: fixed

+ 机器人碰撞严重，且碰撞后完全无法恢复；\
  03/12: 大部分解决，偶尔有碰撞发生（约两局可能有 1 次），但仍无法恢复

+ 发现跳帧，推测为计算量突然上升\
  未实测，可能是集中寻路造成的

+ ~~到一定时间程序就崩溃，可能是内存泄露或者遇到死循环~~\
  03/11: fixed

+ 寻路算法可能效率不够\
  03/12: 暂时不改
+ 考虑A*算法？
+ https://github.com/CHH3213/chhRobotics_CPP/tree/master/PathPlanning/A_Star

+ 避障算法无法解决多物体碰撞\
  03/12: almost fixed


+ 船对泊位选择有问题，无法出发\
  03/12: almost fixed

+ 少了一艘船？

+ 尝试改进为机器人分配泊位的算法：对每个物品，对每个泊位寻路，然后分配

+ 机器人窄道避让效果不是很好，出现了多个机器人碰撞死机和两个机器人循环的情况

+ 算法分配时间过长，计算量集中在某一帧，导致周期性地出现一帧需要1000-2000ms的情况