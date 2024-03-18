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
测试脚本
```
@echo off
cd /D "本地判题器所在目录"
for /R "本地地图所在目录" %%F in (*) do (
   echo start
   echo.
   <NUL set /p "=%%~nF : "
   .\PreliminaryJudge_win_1.exe -m "%%F" "本地可执行文件所在目录" -l NONE  
   echo end
   echo.
)
echo all tested
pause

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
  03/12: 暂时不改\
  03/13: 可以在一次 bfs 内（从机器人位置出发）对所有物品寻路，会极大减少 bfs 次数\
  另外，可以考虑初始化时（以低分辨率）跑出全源最短路

+ 考虑A*算法？\
  https://github.com/CHH3213/chhRobotics_CPP/tree/master/PathPlanning/A_Star\
  03:13: 写了，能用，但是目前还是用的bfs

+ 避障算法无法解决多物体碰撞\
  03/12: almost fixed


+ 船对泊位选择有问题，无法出发\
  03/12: almost fixed

+ 少了一艘船？

+ 尝试改进为机器人分配泊位的算法：对每个物品，对每个泊位寻路，然后分配

+ 机器人窄道避让效果不是很好，出现了多个机器人碰撞死机和两个机器人循环的情况

+ 算法分配时间过长，计算量集中在某一帧，导致周期性地出现一帧需要1000-2000ms的情况

---

1. 碰撞处理（可以高价值优先）

2. 预处理：枚举泊位选择方案，根据（最近泊位距离最大值，机器人分配不均匀程度，泊位运输时间）选择泊位\
03/15：函数preAssign写好，后期调参 
3. 当三倍泊位运输时间+当前时间大于15000时，当前还在泊位的船只最多只能在进行一次有效运输，当当前船只走后机器人做的都是无效运输，期望优化这部分机器人