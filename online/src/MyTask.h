#pragma once
#include "TcpConnection.h"
#include "Mydict.h"
#include "EditDistance.h"

#include <queue>
#include <vector>
#include <set>
using std::priority_queue;
using std::vector;
using std::set;

namespace wd
{

//Mydict * Mydict::_pInstance = nullptr;  //静态成员声明

struct MyResult
{
    MyResult(const string & word, int iFreq, int iDist)
    : _word(word)
    , _iFreq(iFreq)
    , _iDist(iDist)
    {}

    string _word;  //候选词
    int _iFreq;  //词频
    int _iDist;  //与查询词的最小编辑距离
};

struct MyCompare
{
    bool operator()(const MyResult & lhs, const MyResult & rhs)
    {
        if(lhs._iDist != rhs._iDist)
        {
            return lhs._iDist > rhs._iDist;
        }else{
            if(lhs._iFreq != rhs._iFreq)
            {
                return lhs._iFreq < rhs._iDist;
            }else{
                //return !lhs._word.compare(rhs._word);
                return lhs._word > rhs._word;
            }
        }
    }
};

class MyTask
{
public:
    MyTask(const string & queryWord, const TcpConnectionPtr & conn);
    
    void execute();  //执行查询

private:
    void queryIndexTable();  //查询索引，根据索引查询字典
    //void statistic(set<int> & iset);  //进行计算
    //int distance(const string & rhs);  //计算最小编辑距离
    void response(/*Cache & cache*/);  //响应客户端请求
    
private:
    string _queryWord;  //等待查询的单词
    TcpConnectionPtr _conn;  //与客户端的一个连接
    priority_queue<MyResult, vector<MyResult>, MyCompare> _resultQue;//保存候选结果集的优先队列
};

}//end of namespace wd
