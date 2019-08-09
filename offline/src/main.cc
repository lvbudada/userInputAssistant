#include "Configuration.h"
#include "DictProducer.h"
#include "IndexProducer.h"

int main()
{
    //string confPath("../conf/my.conf");
    //wd::Configuration conf(confPath);
    //conf.analysis();
    //conf.show();

    //wd::DictProducer dict(conf);
    //dict.show_files();
    //dict.build_dict();
    //dict.build_cn_dict();

    //wd::IndexProducer index("../data/en/dict.dat", "../data/en/index.dat");
    //index.readDict();
    //index.build_index();
    
    wd::IndexProducer index("../data/cn/dict.dat", "../data/cn/index.dat");
    index.readDict();
    index.build_cn_index();

    return 0;
}

