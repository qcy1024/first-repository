#include <iostream>
#include "solution.h"
#include "router.h"
#include "global.cpp"
using namespace std;

Router router[maxn];
bool check[maxn];
bool connected[maxn][maxn];

int main(int argc, char *argv[])
{
    Solution s;
    s.buildInitState(router,check,connected);
    s.start(router,check,connected);

    return 0;
}
