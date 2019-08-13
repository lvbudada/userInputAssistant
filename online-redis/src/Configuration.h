#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::ifstream;
using std::istringstream;

namespace wd
{

class Configuration
{
public:
    Configuration(const string & filepath)
    : _filepath(filepath)
    {
        analysis();
    }

    //解析配置文件
    void analysis()
    {
        ifstream ifs(_filepath);
        if(!ifs)
        {
            cout << " >> ifstream open " << _filepath << " error" << endl;
            return;
        }

        string line, word;
        while(getline(ifs, line))
        {
            istringstream iss(line);
            iss >> word;
            iss >> _configMap[word];
            //string key, value;
            //iss >> key >> value;
            //_configMap.insert(make_pair(key, value));
        }
        ifs.close();
    }

    map<string, string> & getConfigMap()
    {
        return _configMap;
    }

    //打印读取的配置信息，测试用
    void show()
    {
        for(auto & elem : _configMap)
        {
            cout << elem.first << " --> " << elem.second << endl;
        }
    }
private:
    string _filepath;  //配置文件路径
    map<string, string> _configMap;  //配置文件内容
};

}//end of namespace wd
