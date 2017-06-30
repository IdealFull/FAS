#ifndef FAS_MULTIPROCESSTCPSERVER_H
#define FAS_MULTIPROCESSTCPSERVER_H
#include <boost/noncopyable.hpp>
#include <vector>

#include <PipeFd.h>
#include <EventLoop.h>
#include <TcpServer.h>

namespace fas {

class ProcessTcpServer;

class MultiProcessTcpServer : boost::noncopyable {
public:
    MultiProcessTcpServer();
    ~MultiProcessTcpServer();

    void signalHandler(int signo);
    
    bool reloadInfo();

    bool start();
private:
    PipeFd *pipes_;
    std::vector<ProcessTcpServer *> process_;
    std::vector<pid_t> pids_;
    int threadNum_;
    EventLoop *loop_;
    TcpServer *server_;
    bool quit_;
    bool waiting_;
    sigset_t maskset_;
    sigset_t maskold_;
    sigset_t waitset_;
};

}

#endif
