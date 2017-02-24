#ifndef FAS_EXECUTOR_H
#define FAS_EXECUTOR_H
#include <iostream>
#include <boost/function.hpp>

#include <Types.h>
#include <Timestamp.h>
#include <Events.h>

#define STATE_ADD 1
#define STATE_MOD 2
#define STATE_DEL 3
#define STATE_LOOP 4

class EventLoop;
using namespace std;

class Handle {
public:
  typedef boost::function<void (Events*, Timestamp)> events_handle_t;

  Handle(EventLoop* loop, Events event);
  virtual ~Handle();

  EventLoop* getLoop();
  
  Events* getEvent() const;
  void updateEvent(Events& event);

  void setState(uchar state);
  uchar getState();

  void setHandleRead(const events_handle_t& handle_read);
  void setHandleWrite(const events_handle_t& handle_write);
  void setHandleError(const events_handle_t& handle_error);
  void setHandleClose(const events_handle_t& handle_close);

  void handleEvent(Events*, Timestamp);
private:
  EventLoop *loop_;
  Events *event_;
  uint state_;
  boost::function<void (Events*, Timestamp)> handle_read_event_;
  boost::function<void (Events*, Timestamp)> handle_write_event_;
  boost::function<void (Events*, Timestamp)> handle_error_event_;
  boost::function<void (Events*, Timestamp)> handle_close_event_;
};

#endif // FAS_EXECUTOR_H
