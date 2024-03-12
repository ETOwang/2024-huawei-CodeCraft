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

杂七杂八的优化：

+ 最后可以尽快让船出发

问题：

+ 到某个时间点地图上所有活动均停止，推测出现了死循环。
+ 机器人碰撞严重，且碰撞后完全无法恢复
+ 船对泊位选择有问题，无法出发。
+ 发现跳帧，推测为计算量突然上升
+ 到一定时间程序就崩溃，可能是内存泄露或者遇到死循环
+ 寻路算法可能效率不够（每次1000ms）
+ 避障算法无法解决多物体碰撞