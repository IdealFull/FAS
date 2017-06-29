#ifndef FAS_PROCESSTCPSERVER_H
#define FAS_PROCESSTCPSERVER_H
#include <boost/noncopyable.hpp>

#include <PipeFd.h>
#include <TcpServer.h>
#include <Handle.h>
#include <EventLoop.h>

namespace fas {

class ProcessTcpServer : boost::noncopyable {
public:
    ProcessTcpServer(TcpServer* server, PipeFd *pipe, EventLoop *loop);
    ~ProcessTcpServer();
    bool start();
private:
    TcpServer *server_;
    EventLoop *loop;
    int threadNum_;
    PipeFd *pipe_;
    Handle *handle_;
};

#endif
