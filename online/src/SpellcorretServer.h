#pragma once
#include "Configuration.h"
#include "TcpServer.h"
#include "Threadpool.h"
#include "Mydict.h"

namespace wd
{

//Mydict * Mydict::_pInstance = nullptr;

class SpellcorretServer
{
public:
    SpellcorretServer(const string & confFilePath);

    void start();  //开始提供服务

    void onConnection(const TcpConnectionPtr & conn);  //被注册回调函数，提供给TcpServer使用
    void onMessage(const TcpConnectionPtr & conn);  //被注册回调函数，提供给TcpServer使用
    void onClose(const TcpConnectionPtr & conn);  //被注册回调函数，提供给TcpServer使用

private:
    Configuration  _conf;  //配置文件对象
    TcpServer _tcpServer;  //网络编程对象
    Threadpool _threadpool;  //线程池对象
    //Mydict _dict;  //词典和索引文件
};

}//end of namespace wd
