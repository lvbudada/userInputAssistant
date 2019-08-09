#include "Mydict.h"

#include <fstream>
#include <sstream>
#include <iostream>
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cout;
using std::endl;

namespace wd
{

Mydict::Mydict()
{
    init("../data/en_dict.dat", "../data/cn_dict.dat");
}

Mydict * Mydict::getInstance()
{
    if(_pInstance == nullptr)
    {
        _pInstance = new Mydict();
    }
    return _pInstance;
}

void Mydict::destory()
{
    if(_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

void Mydict::init(const string & dictEnPath, const string & dictCnPath)
{
    readFromDict(dictEnPath);
    readFromDict(dictCnPath);
    //建立索引表
    createIndexTable();
}

void Mydict::readFromDict(const string & filepath)
{
    ifstream ifs(filepath);
    if(!ifs)
    {
        cout << " >> ifstream open " << filepath << " error!" << endl;
        return;
    }

    string line;
    while(getline(ifs, line))
    {
        istringstream iss(line);
        string key;
        int value;
        iss >> key >> value;
        _dict.push_back(make_pair(key, value));
    }
    ifs.close();
}

void Mydict::createIndexTable()
{
    int cnt = 0;
    for(auto it = _dict.begin(); it != _dict.end(); ++it)
    {
        string word = (*it).first;
        int n;
        for(size_t i = 0; i < word.size();)
        {
            n = nBytesCode(word[i]);
            _indexTable[word.substr(i, n)].insert(cnt);
            i += n;
        }
        ++cnt;
    }
}

size_t Mydict::nBytesCode(const char ch)
{
    if(ch & (1 << 7))
    {
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx)
        {
            if(ch & (1 << (6 - idx)))
            {
                ++nBytes;
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

void Mydict::showIndex()
{
    string storeFile = "../data/index.dat";
    ofstream ofs(storeFile);
    if(!ofs)
    {
        cout << " >> ofstream open " << storeFile << " error" << endl;
        return;
    }

    for(auto & elem : _indexTable)
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

}//end of namespace wd

