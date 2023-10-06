#include <cstdio>
#include <iostream>
#include "router.h"
#include "packet.h"
#include "global.cpp"

Router::Router()
{

}

Router::Router(vector<routeTableClause> routeTable1, int id, string ipAddress,
                string bufferIn, string bufferOut,string directConnectedNet)
{
    this->routeTable = routeTable1;
    this->id = id;
    this->ipAddress = ipAddress;
    this->bufferIn = bufferIn;
    this->bufferOut = bufferOut;
    this->directConnectedNet = directConnectedNet;
    this->routeTableReceived.from = "";
}

string Router::getIpAddress()
{
    return this->ipAddress;
}

void Router::setSta(vector<routeTableClause> routeTable1, int id, string ipAddress,
                      string bufferIn, string bufferOut,string directConnectedNet)
{
    this->routeTable = routeTable1;
    this->id = id;
    this->ipAddress = ipAddress;
    this->bufferIn = bufferIn;
    this->bufferOut = bufferOut;
    this->directConnectedNet = directConnectedNet;
}

//打印缓冲队列中的路由表表项
void Router::printQueue()
{
    queue<_routeTableReceived> q = this->updateBuffer;
    cout << "Router" << this->id << " Buffer:\n";
    printf("\t   Aim Address\t       Mask\t  Next Id\t     Next IP\t  Distance\n");
    while( !q.empty() ) {
        vector<routeTableClause> t = q.front().info;
        q.pop();
        for( vector<routeTableClause>::iterator it = t.begin(); it!=t.end(); ++it ) {
            std::cout << "\t  " << it->aimAddress << "\t  " << it->mask << "\t    "
                      << it->nextRouterId << "\t\t  " << it->nextAddress << "\t     " << it->distance << endl;
//            cout << "\t  " << it->aimAddress << "\t  " << it->mask << "\t  "
//                 << it->nextRouterId << "\t\t  " << it->nextAddress <<  "\t  " << it->distance << endl;
        }
    }
    cout << endl;
}

//打印路由器信息
void Router::printRouteTable()
{
    std::cout << "\t\tRouter" << this->id << ":"  << ", connected with"
              << this->directConnectedNet << "\nRouting Table：\n";
    printf("\t   Aim Address\t       Mask\t  Next Id\t     Next IP\t  Distance\n");
    for( vector<routeTableClause>::iterator it = this->routeTable.begin();
         it != this->routeTable.end(); ++it )
    {
        std::cout << "\t  " << it->aimAddress << "\t  " << it->mask << "\t    "
                  << it->nextRouterId << "\t\t  " << it->nextAddress << "\t     " << it->distance;
        std::cout << endl;
    }
    std::cout << endl << endl;
}

string to2(string s)
{
    //将s转换成二进制的字符串，用aim返回
    string aim = "";
    string temps = "";
    string ret = "";
    int t = 0;
    char c = ' ';
    for( unsigned i=0; i<s.size();  ) {
        while( s[i] != '.' && i<s.size() ) {
            t = t * 10 + s[i] - '0' ;
            i++;
        }
        temps = "";
        while( t ) {
            c = t % 2 + '0';
            temps += c;
            t /= 2;
        }

        //满足八位的要求
        while( temps.size() < 8 ) {
            temps += "0";
        }

        for( unsigned i=0; i<=temps.size()/2; i++ ) {
            int tt = temps[i];
            temps[i] = temps[temps.size()-1-i];
            temps[temps.size()-1-i] = tt;
        }
        aim += temps;
        i++;
    }
    return aim;
}

string bitAnd(string s1,string s2)
{
    //先把s1，s2转换成二进制
    s2 = "11111111111111111111111100000000";
    string ret = "";
    s1 = to2(s1);
    int a = 0;
    int b = 0;
    for( unsigned i=0; i<s1.size(); i++ ) {
        a = s1[i] - '0';
        b = s2[i] - '0';
        if( a == 1 && b == 1 ) {
            ret += '1';
        }
        else ret += '0';
    }
    return ret;
}

//转发分组
bool Router::sendPacket(const Packet& p, int& r)
{
    if( p.count <= 0 ) {
        std::cout << "分组不可达！" << std::endl;
        return true;
    }

    //先查找与路由器直接相连的网络
    if( to2(this->directConnectedNet) == bitAnd(p.packetAimAddress,"255.255.255.0") ) {
        cout << "Aim address is directed with this router, give packet to net " << this->directConnectedNet << endl;
        return true;
    }
    //如果没有与目标网络直接相连,查找路由表
    vector<routeTableClause> table = this->routeTable;
    for( vector<routeTableClause>::iterator it=table.begin(); it!=table.end(); ++it ) {
        //让packet中的目标地址与表项中的子网掩码按位与
        string result = bitAnd(p.packetAimAddress,it->mask);

         //如果得到的结果是表项中的目标地址，则交付给那个编号的路由器
        if( result == to2(it->aimAddress) ) {
            std::cout << "Send to Router" << it->nextRouterId << " which port address is:" << it->nextAddress << std::endl;
            r = it->nextRouterId;
            return false;
        }
    }
    //如果所有表项都失败了，说明分组ip地址有误。。。
    std::cout << "Packet Error " << std::endl;
    return true;
}

//根据其他路由器发来的路由信息更新路由表
bool Router::updateRouteTable()
{
    if( this->updateBuffer.empty() )
        return false;
    //取出队首信息，此信息是一个routeListReceived类型的结构体
    _routeTableReceived fr = this->updateBuffer.front();
    this->updateBuffer.pop();
    //将结构体中info的每一项的nextAddress都改为from,并将距离+1
    for( vector<routeTableClause>::iterator it=fr.info.begin(); it!=fr.info.end(); ++it ) {
        it->nextAddress = fr.from;
        it->nextRouterId = fr.fromId;
        it->distance ++;
    }

    vector<routeTableClause> table = this->routeTable;
    //对于此时info中的每一个表项做判断
    bool add = true;
    for( vector<routeTableClause>::iterator it1=fr.info.begin(); it1!=fr.info.end(); ++it1 ) {
        if( it1->aimAddress == this->directConnectedNet )
            continue;
        add = true;
        //扫描旧表项
        for( vector<routeTableClause>::iterator it2=table.begin(); it2!=table.end(); ++it2 ) {
            //原来表中就有网络N
            if( it2->aimAddress == it1->aimAddress  ) {
                //下一跳是from
                add = false;
                if( it2->nextAddress == fr.from ) {
                    it2->setSta(it1->aimAddress,it1->mask,it1->nextRouterId,
                                it1->nextAddress,it1->distance);
                }
                //下一跳不是from
                else {
                    if( it1->distance < it2->distance ) {
                        it2->setSta(it1->aimAddress,it1->mask,it1->nextRouterId,
                                    it1->nextAddress,it1->distance);
                    }
                }
            }
        }
        //1.原来表中没有目的网络N，把该项目直接添加到路由表中   *这里测试成功
        if( add ) {
            routeTableClause t(it1->aimAddress,it1->mask,it1->nextRouterId,
                                      it1->nextAddress,it1->distance);
            this->routeTable.push_back(t);
        }
    }
    return true;
}

//给所有邻接路由器发送路由表
void Router::sendRouteTable(Router *router,bool connected[maxn][maxn])
{
    //创造出自己的路由表，给所有邻接路由器发送
    if( this->routeTable.empty() )
        return ;
    vector<routeTableClause> myTable = this->routeTable;
    int myId = this->id;
    for( int i=1; i<maxn; i++ ) {
        if( connected[myId][i] ) {
            router[i].routeTableReceived.from = this->ipAddress;
            router[i].routeTableReceived.fromId = this->id;
            router[i].routeTableReceived.info = this->routeTable;
            router[i].updateBuffer.push(router[i].routeTableReceived);
        }
    }
}


bool Router::updateRouteTableAll()
{
    if( this->updateBuffer.empty() )
        return false;
    while( !this->updateBuffer.empty() ) {
        //取出队首信息，此信息是一个routeListReceived类型的结构体
        _routeTableReceived fr = this->updateBuffer.front();
        this->updateBuffer.pop();
        //将结构体中info的每一项的nextAddress都改为from,并将距离+1
        for( vector<routeTableClause>::iterator it=fr.info.begin(); it!=fr.info.end(); ++it ) {
            it->nextAddress = fr.from;
            it->nextRouterId = fr.fromId;
            it->distance ++;
        }

        vector<routeTableClause> table = this->routeTable;
        //对于此时info中的每一个表项做判断
        bool add = true;
        for( vector<routeTableClause>::iterator it1=fr.info.begin(); it1!=fr.info.end(); ++it1 ) {
            if( it1->aimAddress == this->directConnectedNet )
                continue;
            add = true;
            //扫描旧表项
            for( vector<routeTableClause>::iterator it2=table.begin(); it2!=table.end(); ++it2 ) {
                //原来表中就有网络N
                if( it2->aimAddress == it1->aimAddress  ) {
                    //下一跳是from
                    add = false;
                    if( it2->nextAddress == fr.from ) {
                        it2->setSta(it1->aimAddress,it1->mask,it1->nextRouterId,
                                    it1->nextAddress,it1->distance);
                    }
                    //下一跳不是from
                    else {
                        if( it1->distance < it2->distance ) {
                            it2->setSta(it1->aimAddress,it1->mask,it1->nextRouterId,
                                        it1->nextAddress,it1->distance);
                        }
                    }
                }
            }
            //1.原来表中没有目的网络N，把该项目直接添加到路由表中   *这里测试成功
            if( add ) {
                routeTableClause t(it1->aimAddress,it1->mask,it1->nextRouterId,
                                          it1->nextAddress,it1->distance);
                this->routeTable.push_back(t);
            }
        }
    }
    return true;
}
