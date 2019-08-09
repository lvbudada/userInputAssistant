#pragma once
#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

namespace wd
{

class Configuration;

class DictProducer
{
public:
    DictProducer(Configuration & conf);

    void build_dict();    //创建英文词典
    void build_cn_dict();  //创建中文词典
    void store_dict(map<string, int> & dict, const string & filepath);  //将词典写入文件

    void show_files() const;  //查看文件路径，作为测试用

private:
    void get_cn_files(string & dir);  //获取中文文件的绝对路径
    size_t nBytesCode(const char ch); //获取一个字节高位为1的个数

private:
    string _enPath;  //英文语料库文件存放路径
    vector<string> _cnPath;  //中文语料库文件的绝对路经集合
    map<string, int> _enDict;  //英文词典
};

}//end of namespace wd
