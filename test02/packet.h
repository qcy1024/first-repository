#ifndef PACKET_H
#define PACKET_H
#include <string>

class Packet
{
public:
    //�����Ŀ���ַ
    std::string packetAimAddress;

    //ʣ������
    int count;

public:
    Packet();
    Packet(std::string packetAimAddress,int count);
};

#endif // PACKET_H
