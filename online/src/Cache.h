#pragma once
#include <unordered_map>
#include <string>
#include <list>
using std::unordered_map;
using std::string;
using std::list;

namespace wd
{

class Cache
{
public:
    struct CacheNode
    {
        CacheNode(const string & k, const string & v)
        : key(k)
        , value(v)
        {}

        string key;
        string value;
    };
    Cache(int num = 10);  //构造函数
    Cache(const Cache & rhs);  //构造函数

    void addElement(const string & key, const string & value);  //往缓存中添加数据
    string query(const string & key);  //查询缓存信息

    list<CacheNode> & getNodes(); //返回缓存信息
    
    //void readFromFile(const string & filename);  //从文件中读取缓存信息
    //void writeToFile(const string & filename);  //将缓存信息写入到文件中
    //void update(const Cache & rhs);  //更新缓存信息

private:
    size_t _capacity;
    list<CacheNode> _nodes;
    unordered_map<string, list<CacheNode>::iterator> _keys;
};

}//end of namespace wd
