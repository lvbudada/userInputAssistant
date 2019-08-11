#pragma once
#include "Cache.h"

#include <vector>
using std::vector;

namespace wd
{

class CacheManager
{
public:
    static void initCache(const string & filename, int cacheNum);  //从缓存文件中读取缓存信息
    static Cache & getCache(const string & threadName);  //获取某个缓存
    static void periodicUpdateCache();  //定时更新所有缓存

private:
    static vector<Cache> _cacheList;  //0号缓存代表总（热点）缓存，缓存数量和线程个数一致
};

//vector<Cache> CacheManager::_cacheList;

}//end of namespace wd
