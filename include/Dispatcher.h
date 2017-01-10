#ifndef DISPATCHER_H
#define DISPATCHER_H
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include "Epoll.h"
#include "Events.h"
#include "Mutex.h"
#include "MutexLocker.h"
#include "Handle.h"
#include "NetBaseTypes.h"

class Handle;
class Epoll;

using namespace std;
class Dispatcher
{
private:
  static const int init_max_events = 10;
  Mutex mutex;
  Poller *poll;
  vector<Events> revents;
  map<int, shared_ptr<Handle>> handles;
  map<int, shared_ptr<Handle>> update_handles;



public:
  Dispatcher();
  bool add_events(shared_ptr<Handle> handle);
  bool add_events(Handle* handle);
  bool mod_events(shared_ptr<Handle> handle);
  bool mod_events(Handle* handle);
  bool del_events(shared_ptr<Handle> handle);
  bool del_events(Handle* handle);

  shared_ptr<Handle>  get_handle_shared_ptr(int fd);
  shared_ptr<Handle>  get_handle_shared_ptr(Handle *handle);

  void loop();

private:
  bool update_events(shared_ptr<Handle> handle);
};

#endif // DISPATCHER_H
