#include "Cache.h"

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

Cache::Cache(int num)
: _capacity(num)
{}

Cache::Cache(const Cache & rhs)
: _capacity(rhs._capacity)
, _nodes(rhs._nodes)
//, _keys(rhs._keys)   错误
{
    for(auto it = _nodes.begin(); it != _nodes.end(); ++it)
    {
        _keys[it->key] = it;
    }
}

string Cache::query(const string & key)
{
    auto it = _keys.find(key);
    //cout << "it->first = " << it->first << endl;
    if(it != _keys.end())
    {
        string ret = it->second->value;
        //cout << "it->second->value = " << it->second->value << endl;
        _nodes.splice(_nodes.begin(), _nodes, it->second);
        _keys[key] = _nodes.begin();

        cout << "Cache::query ret = " << ret;
        return ret;
    }

    return "";
}

void Cache::addElement(const string & key, const string & value)
{
    auto it = _keys.find(key);
    if(it == _keys.end())
    {
        if(_capacity == _nodes.size())
        {
            //淘汰元素
            string deleteKey = _nodes.back().key;
            _nodes.pop_back();
            _keys.erase(deleteKey);
        }

        CacheNode node(key, value);
        _nodes.push_front(node);
        _keys.insert(std::make_pair(key, _nodes.begin()));
        //cout << "Cache::addElement _node.begin().value = " << _nodes.begin()->value;
    }else{
        //更新值
        it->second->value = value;
        _nodes.splice(_nodes.begin(), _nodes, it->second);
        _keys[key] = _nodes.begin();
    }

    std::cout << "data add in cache" << std::endl;
}

list<Cache::CacheNode> & Cache::getNodes()
{
    return _nodes;
}

}//end of namespace wd
