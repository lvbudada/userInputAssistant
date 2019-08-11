#include "SpellcorretServer.h"
#include "Configuration.h"
#include "CacheManager.h"
#include "TimerThread.h"

vector<wd::Cache> wd::CacheManager::_cacheList;
wd::Mydict * wd::Mydict::_pInstance = nullptr;  //类的静态成员必须类外定义

int main()
{   
    wd::Configuration conf("../conf/my.conf");
    
    wd::CacheManager::initCache(conf.getConfigMap()["cache"], stoi(conf.getConfigMap()["threadNum"]));

    wd::TimerThread timer(60, 30, wd::CacheManager::periodicUpdateCache);
    timer.start();
    
    wd::SpellcorretServer server(conf);

    server.start();
    timer.start();
    return 0;
}

