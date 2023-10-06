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

    vector<routeTableClause> routeTable;  //��·����·�ɱ�
    int id;         //·�������
    string ipAddress;   //·����һ���˿ڵ�ip��ַ
    string directConnectedNet;  //���·����ֱ�����������磬����ֻ��һ��
    string bufferIn;    //ģ��·�������뻺����
    string bufferOut;   //ģ��·�������������
    //�ݴ��յ���·����Ϣ
    struct _routeTableReceived {
        vector<routeTableClause> info;
        string from;    //���ͷ�ip
        int fromId;     //���ͷ����
    };
    _routeTableReceived routeTableReceived;
    queue<_routeTableReceived> updateBuffer; //�����ݴ�����ͬ·���������ı�����Ϣ,��ʱÿ��ȡ��ͷ����Ϣ���д��������꼴����

public:
    //���캯��
    Router();
    Router(vector<routeTableClause> routeTable1,int id,string ipAddress,
           string bufferIn,string bufferOut,string directConnectedNet);

    string getIpAddress();

    //����·����״̬
    void setSta(vector<routeTableClause> routeTable1,int id,string ipAddress,
                string bufferIn,string bufferOut,string directConnectedNet);
    //��ӡ�����µ�·�ɱ����
    void printQueue();

    //��ӡ·������Ϣ
    void printRouteTable();

//    ���ݵ�ǰrouteListReceived�е�·����Ϣ����·�ɱ�
    bool updateRouteTable();

    //ȫ������
    bool updateRouteTableAll();

//    �������ڽ�·��������·�ɱ�
    //�������ڽ�·������updateBuffer���ӷ���·�����еı�����Ϣ
    void sendRouteTable(Router* router,bool connected[50][50]);

    //ת������
    bool sendPacket( const Packet& p,int& r );

};

#endif // ROUTER_H
