#include "CacheManager.h"

#include <json/json.h>

#include <fstream>
#include <iostream>
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

namespace wd
{

//vector<Cache> CacheManager::_cacheList;

void CacheManager::initCache(const string & filename, int cacheNum)
{
    ifstream ifs(filename);
    if(!ifs)
    {
        std::cout << ">> ifstream open " << filename << " error!!" << std::endl;
    }

    //ifs.seekg(0, std::ios::end);
    //int fileSize = ifs.tellg();
    //char buff[fileSize+1] = {0};
    //ifs.read(buff, fileSize);
    Json::Reader reader;
    Json::Value root;
    if(!reader.parse(ifs, root))
    {
        std::cout << "Json::Reader reader.parse error!!" << std::endl;
        return;
    }

    Cache cache(10);
    Json::StyledWriter writer;
    for(auto it = root.begin(); it != root.end(); ++it)
    {
        string key = it.memberName();
        string value = writer.write(*it);
        cout << "key = " << key << endl;
        cout << "value = " << value;
        cache.addElement(key, value);
    }

    do{
        _cacheList.push_back(cache);
    }while(cacheNum--);

    cout << "_cacheList.size() = " << _cacheList.size() << endl;
    //string test = _cacheList[0].query("string");
    ifs.close();

    std::cout << "init cache finish" << std::endl;
}

Cache & CacheManager::getCache(const string & threadName)
{
    cout << "CacheManager::getCache::stoi(threadName) = " << stoi(threadName) << endl;
    return _cacheList[stoi(threadName)];
}

//定时更新所有缓存，在TimerThread中执行
void CacheManager::periodicUpdateCache()
{
    //将1开始的缓存写入0
    for(int i = 1; i <= 5; ++i)
    {
        list<Cache::CacheNode> & _nodes = _cacheList[i].getNodes();
        int num = _nodes.size() > 2 ? 2 : _nodes.size();
        auto it = _nodes.begin();
        for(int j = 0; j < num; ++j, ++it)
        {
            _cacheList[0].addElement(it->key, it->value);
        }
    }

    //将0号缓存写入每个cache
    list<Cache::CacheNode> nodes = _cacheList[0].getNodes();
    for(int i = 1; i <= 5; ++i)
    {
        for(auto it = nodes.begin(); it != nodes.end(); ++it)
        {
            _cacheList[i].addElement(it->key, it->value);
        }
    }

    cout << "CacheManager::periodicUpdateCache timerThread 将0号数据回写到每个缓存" << endl;

    //将0号缓存写入文件
    Json::Value root;
    Json::Reader reader;
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        Json::Value temp;
        reader.parse(it->value, temp);
        root[it->key] = temp;
    }

    Json::StyledWriter styleWriter;
    string fileContent = styleWriter.write(root);

    ofstream ofs("../data/cache.dat");
    ofs << fileContent;

    cout << "0号缓存写入文件" << endl;
    ofs.close();
}

}//end of namespace wd
