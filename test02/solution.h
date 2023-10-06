#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include "router.h"

using namespace std;

class Solution
{
public:
    //建立初始网络状态
    void buildInitState(Router router[],bool check[],bool conneckted[50][50]);
    //模拟分组转发
    void sendPacket(Router router[]);
    //开始模拟
    void start(Router router[],bool check[],bool connected[50][50]);
    //打印所有路由器信息
    void printAll(Router router[],bool check[]);
    //添加路由器
    bool addNewRouter(Router router[],bool check[],bool connected[50][50],
                    string routerIp,string netIp);
    //连接两个路由器
    bool connectTwoRouter(Router router[],int id1,int id2,bool check[],bool connected[50][50]);
    Solution();

};

#endif // SOLUTION_H
