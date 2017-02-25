#ifndef FAS_MUTEXLOCKER_H
#define FAS_MUTEXLOCKER_H
#include <pthread.h>


class Mutex;

class MutexLocker {
public:
  MutexLocker(Mutex& mutex);
  ~MutexLocker();
private:
  Mutex& mutex_;
};

#endif // FAS_MUTEXLOCKER_H
