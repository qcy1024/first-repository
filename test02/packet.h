#ifndef PACKET_H
#define PACKET_H
#include <string>

class Packet
{
public:
    //分组的目标地址
    std::string packetAimAddress;

    //剩余跳数
    int count;

public:
    Packet();
    Packet(std::string packetAimAddress,int count);
};

#endif // PACKET_H
