#include "Dispatcher.h"
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

Dispatcher::Dispatcher():
                        poll(NULL){
  revents.reserve(init_max_events);
  poll = EventsPoller::Poller_create();
  assert(poll);
}


bool Dispatcher::Dispatcher_update_events(shared_ptr<Executor> exec) {
  MutexLocker lock(mutex);(void)lock;
  update_execs.insert({exec->Executor_get_event().Events_get_fd(), exec});
  return true;
}


bool Dispatcher::Dispatcher_add_events(shared_ptr<Executor> exec) {
  exec->Executor_set_state(EXECUTOR_STATE_ADD);
  return Dispatcher_update_events(exec);
}
bool Dispatcher::Dispatcher_add_events(Executor* exec) {
  shared_ptr<Executor> exec_ptr(exec);
  return Dispatcher_add_events(exec_ptr);
}

//FIXME:mod by fd
bool Dispatcher::Dispatcher_mod_events(shared_ptr<Executor> exec) {
  exec->Executor_set_state(EXECUTOR_STATE_MOD);
  return Dispatcher_update_events(exec);
}
bool Dispatcher::Dispatcher_mod_events(Executor* exec) {
  //shared_ptr<Executor> exec_ptr = execs.find(exec->Executor_get_event().Events_get_fd())->second;
  shared_ptr<Executor> exec_ptr = Dispatcher_get_exec_shared_ptr(exec);
  return Dispatcher_mod_events(exec_ptr);
}

//FIXME:del by fd
bool Dispatcher::Dispatcher_del_events(shared_ptr<Executor> exec) {
  exec->Executor_set_state(EXECUTOR_STATE_DEL);
  return Dispatcher_update_events(exec);
}
bool Dispatcher::Dispatcher_del_events(Executor* exec) {
  //shared_ptr<Executor> exec_ptr = execs.find(exec->Executor_get_event().Events_get_fd())->second;
  shared_ptr<Executor> exec_ptr = Dispatcher_get_exec_shared_ptr(exec);
  return Dispatcher_del_events(exec_ptr);
}


shared_ptr<Executor>  Dispatcher::Dispatcher_get_exec_shared_ptr(int fd) {
  map<int, shared_ptr<Executor>>::iterator index =  execs.find(fd);
  return index->second;
}

shared_ptr<Executor>  Dispatcher::Dispatcher_get_exec_shared_ptr(Executor *exec) {
  map<int, shared_ptr<Executor>>::iterator index =  execs.find(exec->Executor_get_event().Events_get_fd());
  return index->second;
}


void Dispatcher::Dispatcher_loop() {
  while (true) {

    for(map<int, shared_ptr<Executor>>::iterator index = update_execs.begin(); \
                index != update_execs.end(); index++) {

      assert(index->first == \
             index->second->Executor_get_eventpointer()->Events_get_fd());

      shared_ptr<Executor> op_exec = index->second;

      if (op_exec->Executor_get_state() == EXECUTOR_STATE_ADD) {

        poll->Poller_event_add(index->second->Executor_get_eventpointer());
        //insert
        execs[index->first] = index->second;
				index->second->Executor_set_dispatcher(this);
      } else if (op_exec->Executor_get_state() == EXECUTOR_STATE_MOD) {

        poll->Poller_event_mod(index->second->Executor_get_eventpointer());
        //mod
        execs[index->first] = index->second;
      } else if (op_exec->Executor_get_state() == EXECUTOR_STATE_DEL) {
        poll->Poller_event_del(op_exec->Executor_get_eventpointer());
        //del
        execs.erase(op_exec->Executor_get_event().Events_get_fd());
      } else {
        //error
        assert(false);
      }
    }
    update_execs.clear();
    revents.clear();

    int ret = poll->Poller_loop(revents, 20, -1);

    for(int i = 0; i < ret; i++) { 
      int fd = revents.data()[i].Events_get_fd();
     
      //exec will decreament reference after for end!
      shared_ptr<Executor> exec = execs.find(fd)->second;

      assert(exec->Executor_get_eventpointer()->Events_get_fd() == fd);
      assert(exec->Executor_get_state() != EXECUTOR_STATE_DEL);
      //handle revents
      exec->Executor_handle_event(&(revents.data()[i]));
    }
  }
}

