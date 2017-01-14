#include <TcpServer.h>
#include <iostream>
#include <Socket.h>
#include <unistd.h>
#include <boost/bind.hpp>
#include <Dispatcher.h>
#include <HttpResponse.h>
#include <Timestamp.h>

using namespace std;

TcpServer::TcpServer(NetAddress addr) {
  handle_ = new Handle;

  server_socket_ = socket(AF_INET, SOCK_STREAM, 0);

  SocketBind(server_socket_, addr);

  SocketListen(server_socket_, 50);

  Events serevn(server_socket_, EPOLLIN);

  handle_->set_event(serevn);
}

TcpServer::TcpServer() {
  handle_ = new Handle;

  server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  NetAddress addr(8899, "127.0.0.1");

  SocketBind(server_socket_, addr);

  SocketListen(server_socket_, 50);

  Events serevn(server_socket_, kReadEvent);

  handle_->set_event(serevn);
}


bool TcpServer::init(Dispatcher *dispatcher) {
  this->dispatch_ = dispatcher;
  handle_->set_handle_read(boost::bind(&TcpServer::handle_read_event, this, _1, _2));
  dispatch_->addHandle(handle_);

  return true;
}


inline Dispatcher* TcpServer::get_dispatcher() {
  return dispatch_;
}


void TcpServer::handle_read_event(Events* event, Timestamp time) {
    cout << time.toFormattedString() << " sd = " << event->get_fd() << endl;

    int sd = accept(event->get_fd(), NULL, NULL);

    if (sd == -1) {
      return ;
    }
}
