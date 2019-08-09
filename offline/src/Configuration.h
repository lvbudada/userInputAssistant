#pragma once
#include <stdio.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;

namespace wd
{

class Configuration
{
public:
    Configuration(const string & filepath) 
    : _filepath(filepath)
    {}
    
    //解析配置文件
    void analysis()
    {
        ifstream ifs(_filepath);
        if(!ifs)
        {
            cout << " >> ifstream open " << _filepath << " error!" << endl;
            return;
        }
        //读取配置文件
        string line;
        while(getline(ifs, line))
        {
            _config.push_back(line);
        }
        ifs.close();
    }
    //获取存放配置文件内容的map
    vector<string> & getConfig()
    {
        return _config;
    }

    //打印配置文件内容
    void show()
    {
        for(auto & elem : _config)
        {
            cout << elem << endl;
        }
    }
private:
    string _filepath;   //配置文件路径
    vector<string> _config;  //配置文件内容
};

}//end of namespace wd
