#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <fstream>
#include <sstream>
using std::vector;
using std::string;
using std::pair;
using std::unordered_map;
using std::set;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cout;
using std::endl;

namespace wd
{

class IndexProducer
{
public:
    IndexProducer(const string & dictPath, const string & indexPath)
    : _dictPath(dictPath)
    , _indexPath(indexPath)
    {}
    
    //读取词典文件
    void readDict()
    {   
        ifstream ifs(_dictPath);
        if(!ifs)
        {
            cout << " >> ifstream open " << _dictPath << " error" << endl;
            return;
        }
        string line, word;
        while(getline(ifs, line))
        {
            istringstream iss(line);
            iss >> word;
            _dict.push_back(word);
        }
    }

    //创建英文索引
    void build_index()
    {
        int cnt = 0;
        for(auto it = _dict.begin(); it != _dict.end(); ++it)
        {
            string word = *it;
            for(size_t i = 0; i != word.size(); ++i)
            {
                _index[string(1, word[i])].insert(cnt);
            }
            ++cnt;
        }

        store_index();
    }

    //创建中文索引
    void build_cn_index()
    {
        int cnt = 0;
        for(auto it = _dict.begin(); it != _dict.end(); ++it)
        {
            string word = *it;
            for(size_t i = 0; i < word.size(); )
            {
                int n = nBytesCode(word[i]);
                _index[word.substr(i, n)].insert(cnt);
                i += n;
            }
            ++cnt;
        }

        store_index();
    }
    void store_index()
    {
        ofstream ofs(_indexPath);
        if(!ofs)
        {
            cout << " >> ofstream open " << _indexPath << " error!" << endl;
            return;
        }
        for(auto & elem : _index)
        {
            ofs << elem.first << " ";
            for(auto & idx : elem.second)
            {
                ofs << idx << " ";
            }
            ofs << endl;
        }
        ofs.close();
    }

private:
    //获取一个字节高位开头为1的个数
    size_t nBytesCode(const char ch)
    {
        if(ch & (1 << 7))
        {
            int nBytes = 1;
            for(int idx = 0; idx != 6; ++idx)
            {
                if(ch & (1 << (6 - idx)))
                {
                    ++nBytes;
                }else
                    break;
            }
            return nBytes;
        }
        return 1;
    }
private:
    string _dictPath;  //词典文件路径
    string _indexPath;  //存储的索引文件路径
    vector<string> _dict;  //读取词典时，存储词典
    unordered_map<string, set<int>> _index;  //存储索引
};

}//end of namespace wd
