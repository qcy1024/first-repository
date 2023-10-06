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
    //������ʼ����״̬
    void buildInitState(Router router[],bool check[],bool conneckted[50][50]);
    //ģ�����ת��
    void sendPacket(Router router[]);
    //��ʼģ��
    void start(Router router[],bool check[],bool connected[50][50]);
    //��ӡ����·������Ϣ
    void printAll(Router router[],bool check[]);
    //���·����
    bool addNewRouter(Router router[],bool check[],bool connected[50][50],
                    string routerIp,string netIp);
    //��������·����
    bool connectTwoRouter(Router router[],int id1,int id2,bool check[],bool connected[50][50]);
    Solution();

};

#endif // SOLUTION_H
