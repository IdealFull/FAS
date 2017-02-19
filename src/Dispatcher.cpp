#include <Dispatcher.h>
#include <Timestamp.h>
#include <Thread.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <Log.h>

int Dispatcher::count_ = 0;

Dispatcher::Dispatcher() :
  poll_(NULL),
  mutex_(),
  cond_(mutex_),
  tid_(gettid()) {
  revents_.reserve(kInitMaxHandle_);
  poll_.reset(new Poller);
  assert(poll_);
  count_++;
}

bool Dispatcher::updateHandle(SHandlePtr handle) {
  MutexLocker lock(mutex_);(void)lock;
  updates_.insert({handle->getEvent().getFd(), handle});
  return true;
}


bool Dispatcher::addHandle(SHandlePtr handle) {
  handle->setState(STATE_ADD);
  handle->setLoop(this);
  return updateHandle(handle);
}
bool Dispatcher::addHandle(HandlePtr handle) {
  SHandlePtr handle_ptr(handle);
  return addHandle(handle_ptr);
}

//FIXME:mod by fd
bool Dispatcher::modHandle(SHandlePtr handle) {
  handle->setState(STATE_MOD);
  assert(handle->loop() == this);
  return updateHandle(handle);
}
bool Dispatcher::modHandle(HandlePtr handle) {
  SHandlePtr handle_ptr = handleSharedPtr(handle);
  return modHandle(handle_ptr);
}

//FIXME:del by fd
bool Dispatcher::delHandle(SHandlePtr handle) {
  handle->setState(STATE_DEL);
  assert(handle->loop() == this);
  return updateHandle(handle);
}
bool Dispatcher::delHandle(HandlePtr handle) {
  SHandlePtr handle_ptr = handleSharedPtr(handle);
  return delHandle(handle_ptr);
}

typename Dispatcher::SHandlePtr
Dispatcher::handleSharedPtr(int fd) {
  map<int, SHandlePtr>::iterator index =  handles_.find(fd);
  return index->second;
}

typename Dispatcher::SHandlePtr
Dispatcher::handleSharedPtr(HandlePtr handle) {
  map<int, SHandlePtr>::iterator index =  \
          handles_.find(handle->getEvent().getFd());
  return index->second;
}

void Dispatcher::assertInDispatch() {
  assert(tid_ == gettid());
}

void Dispatcher::dispatch() {

  //only defined once
  Timestamp looptime;
  while (true) {
    for(auto cur = updates_.begin();
             cur != updates_.end();
             cur++) {

      SHandlePtr handle = cur->second;

      if (handle->getState() == STATE_ADD) {
        poll_->events_add_(handle->getEventPtr());
        //insert
        handles_[cur->first] = handle;
        handle->setState(STATE_LOOP);
      } else if (handle->getState() == STATE_MOD) {
        poll_->events_mod_(handle->getEventPtr());
        //mod
        handles_[cur->first] = handle;
        handle->setState(STATE_LOOP);
      } else if (handle->getState() == STATE_DEL) {
        poll_->events_del_(handle->getEventPtr());
        //del
        handles_.erase(handle->getEvent().getFd());
      } else {
        //error
        assert(false);
      }
    }

    updates_.clear();
    revents_.clear();

    int ret = poll_->loop_(revents_, 20, -1);
    looptime = Timestamp::now();

    cout << "current dispather num is " << Dispatcher::count_ << endl;

    for(int i = 0; i < ret; i++) { 
      int fd = revents_.data()[i].getFd();
     
      //handle will decreament reference after for end!
      SHandlePtr handle = handles_.find(fd)->second;
      if (handle->getState() != STATE_LOOP) {
        LOG_DEBUG("After Loop have handle with state STATE_LOOP! it is unnormal!");
        continue;
      }
      assert(handle->getEventPtr()->getFd() == fd);
      //handle revents
      handle->handleEvent(&(revents_.data()[i]), looptime);
    }
  }
}

Dispatcher::~Dispatcher() {
}

