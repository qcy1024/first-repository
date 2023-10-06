#include <iostream>
#include <cstdio>
#include <string>
#include "solution.h"
#include "global.cpp"
#include <windows.h>
using namespace std;

Solution::Solution()
{

}

//IP网络初始状态的构造
void Solution::buildInitState(Router router[],bool check[],bool connected[maxn][maxn])
{
    //先创造五个路由表，再根据五个路由表创造五个路由器
    vector<routeTableClause> routeTable1;
    vector<routeTableClause> routeTable2;
    vector<routeTableClause> routeTable3;
    vector<routeTableClause> routeTable4;
    vector<routeTableClause> routeTable5;
    //建立路由器1的路由表
    routeTableClause routeTableClause1("192.168.22.0","255.255.255.0",2,"192.168.2.0",1);
    routeTable1.push_back(routeTableClause1);
    routeTableClause1.setSta("192.168.33.0","255.255.255.0",3,"192.168.3.0",1);
    routeTable1.push_back(routeTableClause1);
    routeTableClause1.setSta("192.168.44.0","255.255.255.0",4,"192.168.4.0",1);
    routeTable1.push_back(routeTableClause1);
    //2
    routeTableClause1.setSta("192.168.11.0","255.255.255.0",1,"192.168.1.0",1);
    routeTable2.push_back(routeTableClause1);
    routeTableClause1.setSta("192.168.33.0","255.255.255.0",1,"192.168.1.0",2);
    routeTable2.push_back(routeTableClause1);
    routeTableClause1.setSta("192.168.44.0","255.255.255.0",4,"192.168.4.0",1);
    routeTable2.push_back(routeTableClause1);
    //3
    routeTableClause1.setSta("192.168.11.0","255.255.255.0",1,"192.168.1.0",1);
    routeTable3.push_back(routeTableClause1);
    routeTableClause1.setSta("192.168.22.0","255.255.255.0",1,"192.168.1.0",2);
    routeTable3.push_back(routeTableClause1);
    routeTableClause1.setSta("192.168.44.0","255.255.255.0",4,"192.168.4.0",1);
    routeTable3.push_back(routeTableClause1);
    routeTableClause1.setSta("192.168.55.0","255.255.255.0",5,"192.168.5.0",1);
    routeTable3.push_back(routeTableClause1);
    //4
    routeTableClause1.setSta("192.168.11.0","255.255.255.0",1,"192.168.1.0",1);
    routeTable4.push_back(routeTableClause1);
    routeTableClause1.setSta("192.168.22.0","255.255.255.0",2,"192.168.2.0",1);
    routeTable4.push_back(routeTableClause1);
    routeTableClause1.setSta("192.168.33.0","255.255.255.0",3,"192.168.3.0",1);
    routeTable4.push_back(routeTableClause1);
    //5
    routeTableClause1.setSta("192.168.33.0","255.255.255.0",3,"192.168.3.0",1);
    routeTable5.push_back(routeTableClause1);
    //在模拟数组中建立五个路由器
    router[1].setSta(routeTable1,1,"192.168.1.0","","","192.168.11.0");
    router[2].setSta(routeTable2,2,"192.168.2.0","","","192.168.22.0");
    router[3].setSta(routeTable3,3,"192.168.3.0","","","192.168.33.0");
    router[4].setSta(routeTable4,4,"192.168.4.0","","","192.168.44.0");
    router[5].setSta(routeTable5,5,"192.168.5.0","","","192.168.55.0");
    //初始化连接状态
    for( int i=0; i<maxn; i++ ) {
        for( int j=0; j<maxn; j++ ) {
            connected[i][j] = false;
        }
        check[i] = false;
    }
    connected[1][2] = connected[2][1] = true;
    connected[1][3] = connected[3][1] = true;
    connected[1][4] = connected[4][1] = true;
    connected[2][4] = connected[4][2] = true;
    connected[3][4] = connected[4][3] = true;
    connected[3][5] = connected[5][3] = true;
    check[1] = check[2] = check[3] = check[4] = check[5] = true;
}

//打印所有路由器信息
void Solution::printAll(Router router[],bool check[])
{
    for( int i=1; i<maxn; i++ ) {
        if( check[i] ) {
            router[i].printRouteTable();
        }
    }
}

//模拟分组转发
void Solution::sendPacket(Router router[])
{
    printf("Packet sending:\n");
    string s = "";
    int r = 0;
    cout << "Input aim address and router id:" ;
    cin >> s >> r;
    Packet p(s,16);
    while( !router[r].sendPacket(p,r) ) {
        p.count -= 1;
        std::cout << "Remaining hops: " << p.count << std::endl << std::endl;
        Sleep(1500);
    }
}

//添加新路由器
bool Solution::addNewRouter(Router router[], bool check[], bool connected[maxn][maxn],
                            string routerIp,string netIp)
{
    for( int i=1; i<maxn; i++ ) {
        if( !check[i] ) {
            Router tempRouter;
            vector<routeTableClause> tempT;
            tempRouter.setSta(tempT,i,routerIp,"","",netIp);
            router[i] = tempRouter;
            check[i] = true;
            cout << "\nsuccessfully added new router." << endl;
            Sleep(500);
            router[i].printRouteTable();
            return true;
        }
    }
    cout << "added fail." << endl;
    return false;
}

//连接两个路由器
bool Solution::connectTwoRouter(Router router[],int id1, int id2, bool check[], bool connected[maxn][maxn])
{
    if( ! ( check[id1] && check[id2] ) ) {
        if( !check[id1] ) {
            cout << router[id1].getIpAddress() << " dose not exist." << endl;
        }
        else {
            cout << router[id2].getIpAddress() << " dose not exist." << endl;
        }
        return false;
    }
    if( connected[id1][id2] && connected[id2][id1] ) {
        cout << router[id1].getIpAddress() << " and " << router[id2].getIpAddress() << " has already connected." << endl;
        return false;
    }
    routeTableClause tempT;
    tempT.setSta(router[id2].directConnectedNet,"255.255.255.0",id2,router[id2].getIpAddress(),1);
    router[id1].routeTable.push_back(tempT);
    tempT.setSta(router[id1].directConnectedNet,"255.255.255.0",id1,router[id1].getIpAddress(),1);
    router[id2].routeTable.push_back(tempT);
    connected[id1][id2] = connected[id2][id1] = true;
    cout << router[id1].getIpAddress() << " and " << router[id2].getIpAddress() << "successfully connected." << endl;
    return true;
}

//Start!!
void Solution::start(Router router[],bool check[],bool connected[maxn][maxn])
{
    while( true )
    {
        std::cout << "  Router: ";
        std::string Input = "";
        getline(cin,Input);
        if( Input == "send packet" ) {
            char c = 'n';
            while( true ) {
                this->sendPacket(router);
                cout << "Continue?[y/n]" ;
                cin >> c;
                if( c == 'y' ) continue;
                else break;
            }
        }
        else if( Input == "print all" ) {
            this->printAll(router,check);
        }
        else if( Input == "clear" ) {
            system("cls");
        }
        else if( Input == "send route table" ) {
            int routerid = 0;
            cout << "Input Router Id:" ;
            cin >> routerid;
            router[routerid].sendRouteTable(router,connected);
            cout << routerid << "successfully sended." << endl;
        }
        else if( Input == "send route table all" ) {
            for( int i=1; i<maxn; i++ ) {
                if( check[i] )
                    router[i].sendRouteTable(router,connected);
            }
        }
        else if( Input == "update route table" ) {
            int id = 0;
            cout << "Input Router Id:";
            cin >> id;
            if( router[id].updateRouteTable() ) {
                cout << "Router " << id << " successfully updated." << endl;
            }
            else {
                cout << "Router " << id << "'s Route table is empty. " << endl;
            }
        }
        else if( Input == "update route table all" ) {
            for( int i=0; i<maxn; i++ ) {
                if( check[i] ) {
                    if( router[i].updateRouteTable() ){
                        cout << "Router " << i << " successfully updated." << endl;
                    }
                    else {
                        cout << "Router " << i << "'s Route table is empty. " << endl;
                    }
                }
            }
        }
        else if( Input == "update all route table all" ) {
            for( int i=0; i<maxn; i++ ) {
                if( check[i] ) {
                    if( router[i].updateRouteTableAll() ){
                        cout << "Router " << i << " successfully updated." << endl;
                    }
                    else {
                        cout << "Router " << i << "'s Route table is empty. " << endl;
                    }
                }
            }
        }
        else if( Input == "add new router" ) {
            string routerIp;
            string netIp;
            cout << "input port ip and net ip:";
            cin >> routerIp >> netIp;
            this->addNewRouter(router,check,connected,routerIp,netIp);
        }
        else if( Input == "connect" ) {
            int id1,id2;
            cout << "input Router id:";
            cin >> id1 >> id2;
            this->connectTwoRouter(router,id1,id2,check,connected);
        }
        else if( Input == "print buffer" ) {
            int id = 0;
            cout << "Input Router Id:";
            cin >> id;
            router[id].printQueue();
        }
        else if( Input == "restart" ) {
            this->buildInitState(router,check,connected);
        }
        else if( Input == "q" ) {
            break;
        }
        else if( Input == "later" ) {
            int t = 10;
            while(t--) {
                for( int i=1; i<maxn; i++ ) {
                    if( check[i] )
                        router[i].sendRouteTable(router,connected);
                }
                for( int i=0; i<maxn; i++ ) {
                    if( check[i] ) {
                        ( router[i].updateRouteTableAll() );
                    }
                }
            }
        }
        fflush(stdin);
    }
}
