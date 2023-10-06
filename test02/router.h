#ifndef ROUTER_H
#define ROUTER_H
#include <vector>
#include <string>
#include <queue>
#include "routetableclause.h"
#include "packet.h"
using namespace std;
class Router
{
public:

    vector<routeTableClause> routeTable;  //本路由器路由表
    int id;         //路由器编号
    string ipAddress;   //路由器一个端口的ip地址
    string directConnectedNet;  //与该路由器直接相连的网络，假设只有一个
    string bufferIn;    //模拟路由器输入缓冲区
    string bufferOut;   //模拟路由器输出缓冲区
    //暂存收到的路由信息
    struct _routeTableReceived {
        vector<routeTableClause> info;
        string from;    //发送方ip
        int fromId;     //发送方编号
    };
    _routeTableReceived routeTableReceived;
    queue<_routeTableReceived> updateBuffer; //允许暂存多个不同路由器发来的表项信息,此时每次取队头的信息进行处理，处理完即出队

public:
    //构造函数
    Router();
    Router(vector<routeTableClause> routeTable1,int id,string ipAddress,
           string bufferIn,string bufferOut,string directConnectedNet);

    string getIpAddress();

    //设置路由器状态
    void setSta(vector<routeTableClause> routeTable1,int id,string ipAddress,
                string bufferIn,string bufferOut,string directConnectedNet);
    //打印待更新的路由表表项
    void printQueue();

    //打印路由器信息
    void printRouteTable();

//    根据当前routeListReceived中的路由信息更新路由表
    bool updateRouteTable();

    //全部更新
    bool updateRouteTableAll();

//    向所有邻接路由器发送路由表
    //将所有邻接路由器的updateBuffer增加发送路由器中的表项信息
    void sendRouteTable(Router* router,bool connected[50][50]);

    //转发分组
    bool sendPacket( const Packet& p,int& r );

};

#endif // ROUTER_H
