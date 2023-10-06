#ifndef ROUTELIST_H
#define ROUTELIST_H

#include <string>

class routeTableClause
{
public:
    std::string aimAddress;      //目标网络地址
    std::string mask;            //子网掩码
    int nextRouterId;       //下一跳路由器
    std::string nextAddress;     //下一跳地址
    int distance;       //距离(跳数)

    routeTableClause();
    routeTableClause(std::string aimAddress,std::string mask,int nextRouterId,
                    std::string nextAddress,int distance ) ;
    setSta(std::string aimAddress,std::string mask,int nextRouterId,
           std::string nextAddress,int distance);
};

#endif // ROUTELIST_H
