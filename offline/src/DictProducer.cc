#include "DictProducer.h"
#include "Configuration.h"
#include "cppjieba/Jieba.hpp"

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#include <sstream>
using std::istringstream;
using std::ofstream;

namespace wd
{

DictProducer::DictProducer(Configuration & conf)
: _enPath(*conf.getConfig().begin())
{
    get_cn_files(*(conf.getConfig().begin() + 1));
}

void DictProducer::get_cn_files(string & dir)
{
    DIR * cnDir = opendir(dir.c_str());
    if(!cnDir)
    {
        perror("opendir");
    }
    struct dirent *p;
    while((p = readdir(cnDir)) != NULL)
    {
        if(!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
        {
            continue;
        }
        string cnPath = dir + "/" + p->d_name;
        _cnPath.push_back(cnPath);
    }

    closedir(cnDir);
}

void DictProducer::show_files() const
{
    cout << _enPath << endl;
    for(auto & elem : _cnPath)
    {
        cout << elem << endl;
    }
}

//创建英文词典
void DictProducer::build_dict()
{
    //map<string, int> enDict;  //记录词频
    ifstream ifs(_enPath);
    if(!ifs)
    {
        cout << " >> ifstream open " << _enPath << " error!" << endl;
        return;
    }
    string line, word;
    while(getline(ifs, line))
    {
        for(size_t idx = 0; idx < line.size(); ++idx)
        {
            if(!::isalpha(line[idx])) //不是字母的替换成空格
            {
                line[idx] = ' ';
            }else if(::isupper(line[idx])){  //将大写英文字母转换成小写
                line[idx] = ::tolower(line[idx]);
            }
        }
        istringstream iss(line);
        while(iss >> word)
        {
            ++_enDict[word];
        }
    }
    ifs.close();
    store_dict(_enDict, "../data/en/dict.dat");
}

//创建英文词典
void DictProducer::build_cn_dict()
{
    const char* const DICT_PATH = "dict/jieba.dict.utf8";
    const char* const HMM_PATH = "dict/hmm_model.utf8";
    const char* const USER_DICT_PATH = "dict/user.dict.utf8";
    const char* const IDF_PATH = "dict/idf.utf8";
    const char* const STOP_WORD_PATH = "dict/stop_words.utf8";

    cppjieba::Jieba jieba(DICT_PATH,
            HMM_PATH,
            USER_DICT_PATH,
            IDF_PATH,
            STOP_WORD_PATH);

    map<string, int> cnDict;
    for(auto it = _cnPath.begin(); it != _cnPath.end(); ++it)
    {
        ifstream ifs(*it);
        if(!ifs)
        {
            cout << " >> ifstream open " << *it << " error!" << endl;
            return;
        }

        string line;
        while(getline(ifs, line))
        {
            vector<string> words;
            jieba.Cut(line, words, true);
            for(auto & word : words)
            {
                if(nBytesCode(word[0]) >= 3)
                {
                    ++cnDict[word];
                }
            }
        }
        ifs.close();
    }

    store_dict(cnDict, "../data/cn/dict.dat");
}

//获取一个字节高位开头为1的个数
size_t DictProducer::nBytesCode(const char ch)
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

void DictProducer::store_dict(map<string, int> & dict, const string & filepath)
{
    ofstream ofs(filepath);
    for(auto & elem : dict)
    {
        ofs << elem.first << " " << elem.second << endl;
    }
    ofs.close();
}


}//end of namespace wd
