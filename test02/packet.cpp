#include "packet.h"
#include <string>
Packet::Packet()
{

}

Packet::Packet(std::string packetAimAddress, int count):packetAimAddress(packetAimAddress),count(count) {};

