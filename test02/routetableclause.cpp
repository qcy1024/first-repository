#include "routetableclause.h"
using namespace std;

routeTableClause::routeTableClause()
{

}

routeTableClause::routeTableClause(string aimAddress, string mask,int nextRouterId,
                                 string nextAddress,int distance)
{
    this->aimAddress = aimAddress;
    this->mask = mask;
    this->nextAddress = nextAddress;
    this->nextRouterId = nextRouterId;
    this->distance = distance;
}

routeTableClause::setSta(string aimAddress, string mask, int nextRouterId,
                        string nextAddress,int distance)
{
    this->aimAddress = aimAddress;
    this->mask = mask;
    this->nextAddress = nextAddress;
    this->nextRouterId = nextRouterId;
    this->distance = distance;
}

