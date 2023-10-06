#ifndef ROUTELIST_H
#define ROUTELIST_H

#include <string>

class routeTableClause
{
public:
    std::string aimAddress;      //Ŀ�������ַ
    std::string mask;            //��������
    int nextRouterId;       //��һ��·����
    std::string nextAddress;     //��һ����ַ
    int distance;       //����(����)

    routeTableClause();
    routeTableClause(std::string aimAddress,std::string mask,int nextRouterId,
                    std::string nextAddress,int distance ) ;
    setSta(std::string aimAddress,std::string mask,int nextRouterId,
           std::string nextAddress,int distance);
};

#endif // ROUTELIST_H
