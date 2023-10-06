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

//��ӡ��������е�·�ɱ����
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

//��ӡ·������Ϣ
void Router::printRouteTable()
{
    std::cout << "\t\tRouter" << this->id << ":"  << ", connected with"
              << this->directConnectedNet << "\nRouting Table��\n";
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
    //��sת���ɶ����Ƶ��ַ�������aim����
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

        //�����λ��Ҫ��
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
    //�Ȱ�s1��s2ת���ɶ�����
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

//ת������
bool Router::sendPacket(const Packet& p, int& r)
{
    if( p.count <= 0 ) {
        std::cout << "���鲻�ɴ" << std::endl;
        return true;
    }

    //�Ȳ�����·����ֱ������������
    if( to2(this->directConnectedNet) == bitAnd(p.packetAimAddress,"255.255.255.0") ) {
        cout << "Aim address is directed with this router, give packet to net " << this->directConnectedNet << endl;
        return true;
    }
    //���û����Ŀ������ֱ������,����·�ɱ�
    vector<routeTableClause> table = this->routeTable;
    for( vector<routeTableClause>::iterator it=table.begin(); it!=table.end(); ++it ) {
        //��packet�е�Ŀ���ַ������е��������밴λ��
        string result = bitAnd(p.packetAimAddress,it->mask);

         //����õ��Ľ���Ǳ����е�Ŀ���ַ���򽻸����Ǹ���ŵ�·����
        if( result == to2(it->aimAddress) ) {
            std::cout << "Send to Router" << it->nextRouterId << " which port address is:" << it->nextAddress << std::endl;
            r = it->nextRouterId;
            return false;
        }
    }
    //������б��ʧ���ˣ�˵������ip��ַ���󡣡���
    std::cout << "Packet Error " << std::endl;
    return true;
}

//��������·����������·����Ϣ����·�ɱ�
bool Router::updateRouteTable()
{
    if( this->updateBuffer.empty() )
        return false;
    //ȡ��������Ϣ������Ϣ��һ��routeListReceived���͵Ľṹ��
    _routeTableReceived fr = this->updateBuffer.front();
    this->updateBuffer.pop();
    //���ṹ����info��ÿһ���nextAddress����Ϊfrom,��������+1
    for( vector<routeTableClause>::iterator it=fr.info.begin(); it!=fr.info.end(); ++it ) {
        it->nextAddress = fr.from;
        it->nextRouterId = fr.fromId;
        it->distance ++;
    }

    vector<routeTableClause> table = this->routeTable;
    //���ڴ�ʱinfo�е�ÿһ���������ж�
    bool add = true;
    for( vector<routeTableClause>::iterator it1=fr.info.begin(); it1!=fr.info.end(); ++it1 ) {
        if( it1->aimAddress == this->directConnectedNet )
            continue;
        add = true;
        //ɨ��ɱ���
        for( vector<routeTableClause>::iterator it2=table.begin(); it2!=table.end(); ++it2 ) {
            //ԭ�����о�������N
            if( it2->aimAddress == it1->aimAddress  ) {
                //��һ����from
                add = false;
                if( it2->nextAddress == fr.from ) {
                    it2->setSta(it1->aimAddress,it1->mask,it1->nextRouterId,
                                it1->nextAddress,it1->distance);
                }
                //��һ������from
                else {
                    if( it1->distance < it2->distance ) {
                        it2->setSta(it1->aimAddress,it1->mask,it1->nextRouterId,
                                    it1->nextAddress,it1->distance);
                    }
                }
            }
        }
        //1.ԭ������û��Ŀ������N���Ѹ���Ŀֱ����ӵ�·�ɱ���   *������Գɹ�
        if( add ) {
            routeTableClause t(it1->aimAddress,it1->mask,it1->nextRouterId,
                                      it1->nextAddress,it1->distance);
            this->routeTable.push_back(t);
        }
    }
    return true;
}

//�������ڽ�·��������·�ɱ�
void Router::sendRouteTable(Router *router,bool connected[maxn][maxn])
{
    //������Լ���·�ɱ��������ڽ�·��������
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
        //ȡ��������Ϣ������Ϣ��һ��routeListReceived���͵Ľṹ��
        _routeTableReceived fr = this->updateBuffer.front();
        this->updateBuffer.pop();
        //���ṹ����info��ÿһ���nextAddress����Ϊfrom,��������+1
        for( vector<routeTableClause>::iterator it=fr.info.begin(); it!=fr.info.end(); ++it ) {
            it->nextAddress = fr.from;
            it->nextRouterId = fr.fromId;
            it->distance ++;
        }

        vector<routeTableClause> table = this->routeTable;
        //���ڴ�ʱinfo�е�ÿһ���������ж�
        bool add = true;
        for( vector<routeTableClause>::iterator it1=fr.info.begin(); it1!=fr.info.end(); ++it1 ) {
            if( it1->aimAddress == this->directConnectedNet )
                continue;
            add = true;
            //ɨ��ɱ���
            for( vector<routeTableClause>::iterator it2=table.begin(); it2!=table.end(); ++it2 ) {
                //ԭ�����о�������N
                if( it2->aimAddress == it1->aimAddress  ) {
                    //��һ����from
                    add = false;
                    if( it2->nextAddress == fr.from ) {
                        it2->setSta(it1->aimAddress,it1->mask,it1->nextRouterId,
                                    it1->nextAddress,it1->distance);
                    }
                    //��һ������from
                    else {
                        if( it1->distance < it2->distance ) {
                            it2->setSta(it1->aimAddress,it1->mask,it1->nextRouterId,
                                        it1->nextAddress,it1->distance);
                        }
                    }
                }
            }
            //1.ԭ������û��Ŀ������N���Ѹ���Ŀֱ����ӵ�·�ɱ���   *������Գɹ�
            if( add ) {
                routeTableClause t(it1->aimAddress,it1->mask,it1->nextRouterId,
                                          it1->nextAddress,it1->distance);
                this->routeTable.push_back(t);
            }
        }
    }
    return true;
}
