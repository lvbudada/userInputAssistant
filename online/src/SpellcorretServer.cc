#include "SpellcorretServer.h"
#include "MyTask.h"

namespace wd
{

SpellcorretServer::SpellcorretServer(const string & confFilePath)
: _conf(confFilePath)
, _tcpServer(_conf.getConfigMap()["ip"], (unsigned short)stoi(_conf.getConfigMap()["port"]))
, _threadpool(stoi(_conf.getConfigMap()["threadNum"]), stoi(_conf.getConfigMap()["queSize"]))    
{
    
}

//开始提供服务
void SpellcorretServer::start()
{
    _threadpool.start();

    _tcpServer.setConnectionCallback(std::bind(&SpellcorretServer::onConnection, this, std::placeholders::_1));
    _tcpServer.setMessageCallback(std::bind(&SpellcorretServer::onMessage, this, std::placeholders::_1));
    _tcpServer.setCloseCallback(std::bind(&SpellcorretServer::onClose, this, std::placeholders::_1));

    _tcpServer.start();
}

void SpellcorretServer::onConnection(const TcpConnectionPtr & conn)
{
    cout << conn->toString() << " has connected!" << endl;
    //conn->send("welcome to server .");
}

void SpellcorretServer::onClose(const TcpConnectionPtr & conn)
{
    cout << "onClose...." << endl;
    cout << conn->toString() << " has closed!" << endl;
}

void SpellcorretServer::onMessage(const TcpConnectionPtr & conn)
{
    cout << "onMessage...." << endl;
    string msg = conn->receive();
    cout << ">> receive msg from client: " << msg << endl;
    //业务逻辑的处理交给线程池去处理
    //decode
    //compute
    //encode
    MyTask task(msg, conn);

    _threadpool.addTask(std::bind(&MyTask::execute, task));
}

}//end of namespace wd


