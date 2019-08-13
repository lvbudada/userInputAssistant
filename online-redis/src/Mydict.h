#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>
using std::vector;
using std::string;
using std::map;
using std::set;
using std::pair;

namespace wd
{

class Mydict
{
public:
    static Mydict * getInstance();

    static void destory();

    //通过中文和英文词典文件路径初始化词典
    void init(const string & dictEnPath, const string & dictCnPath);

    vector<pair<string, int>> & getDict()  //获取词典
    {   return _dict; }
    map<string, set<int>> & getIndexTable()  //获取索引表
    {   return _indexTable; }

    void showIndex(); //测试用，存到磁盘上，查看结果
private:
    void readFromDict(const string & filepath);
    void createIndexTable();  //创建索引表
    //获取一个字节高位开头为1的个数
    size_t nBytesCode(const char ch);

private:
    Mydict();
    ~Mydict() {}

private:
    vector<pair<string, int>> _dict;  //词典
    map<string, set<int>> _indexTable;  //索引表
    static Mydict * _pInstance;
};

}//end of namespace wd
