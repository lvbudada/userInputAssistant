#include "MyTask.h"
#include "CacheManager.h"

#include <json/json.h>

#include <iostream>
using std::cout;
using std::endl;

//vector<wd::Cache> wd::CacheManager::_cacheList;

namespace wd
{
//Mydict * Mydict::_pInstance = nullptr;  //类的静态成员必须类外定义
//vector<string> CacheManager::_cacheList;

namespace current_thread
{
    extern __thread const char * threadName;
}

MyTask::MyTask(const string & queryWord, const TcpConnectionPtr & conn)
: _queryWord(queryWord)
, _conn(conn)
{

}

//执行查询
void MyTask::execute()
{
    //首先查询缓存
    if(queryCache())
    {
        cout << "数据在缓存中查到" << endl;
    }else{
        queryIndexTable();

        response();  //将数据封装成json，响应用户
    }

}

bool MyTask::queryCache()
{
    Cache & cache = CacheManager::getCache(current_thread::threadName);
    //cout << "current_thread::threadName = " << current_thread::threadName << endl;
    
    //cout << "MyTask::queryCache result != \"\"" << endl;
    string result = cache.query(_queryWord);
    
    if(result != "")
    {
        int dataLen = result.size();
        string reData = string((char*)&dataLen, 4) + result;
        //cout << reData << endl;
        //cout << "MyTask::queryCache() on cache" << endl;
        _conn->sendInLoop(reData);
        return true;
    }
    return false;
}

//查询索引，根据索引查询字典
void MyTask::queryIndexTable()
{
    //cout << "queryIndexTable" << endl;
    Mydict * dict = Mydict::getInstance();
    //printf("词典的地址：%p\n", dict);
    map<string, set<int>> & indexTable = dict->getIndexTable();
    vector<pair<string, int>> & queryDict = dict->getDict();
    
    set<int> index;
    //查询索引
    for(size_t idx = 0; idx < _queryWord.size();)
    {
        size_t nBytes = nBytesCode(_queryWord[idx]);
        string word = _queryWord.substr(idx, nBytes);
        for(auto it = indexTable[word].begin(); it != indexTable[word].end(); ++it)
        {
            index.insert(*it);
        }
        idx += nBytes;
    }

    //根据索引查询字典
    for(auto it = index.begin(); it != index.end(); ++it)
    {
        string word = queryDict[*it].first;
        int iFreq = queryDict[*it].second;
        int iDist = editDistance(_queryWord, word);
        _resultQue.push(MyResult(word, iFreq, iDist));
    }
}

//响应客户端数据
void MyTask::response()
{
    //cout << "response" << endl;
    size_t reSize = (_resultQue.size() >= 5 ? 5 : _resultQue.size());

    Json::Value jsonData;
    //从优先队列取五个元素
    for(size_t i = 0; i != reSize; ++i)
    {
        string word = _resultQue.top()._word;
        _resultQue.pop();
    
        jsonData[std::to_string(i+1)] = Json::Value(word);
    }
    
    Json::StyledWriter style_writer;
    string strJson = style_writer.write(jsonData);
    int dataLen = strJson.size();
    cout << strJson.size() << endl;
    //cout << std::to_string(strJson.size()) << endl;
    //cout << std::to_string(strJson.size()).size() << endl;
    
    //std::cout << strJson << std::endl;
    //string reData = std::to_string((int)strJson.size()) + strJson;
    string reData = string((char*)&dataLen, 4) + strJson;
    cout << reData << endl;
    //cout << *((short*)(reData.c_str())) << endl;

    //将数据添加到缓存
    Cache & cache = CacheManager::getCache(current_thread::threadName);
    cache.addElement(_queryWord, strJson);
    cout << "MyTask::response 数据添加到缓存" <<endl;

    _conn->sendInLoop(reData);
}

}//end of namespace wd
