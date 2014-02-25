#ifndef _YOHUB_SHARE_QUEUE_H_ 
#define _YOHUB_SHARE_QUEUE_H_

#include "share/mutex.h"
#include "share/condvar.h"
#include <vector>

namespace yohub {

template<typename T>
class Queue {
  public:
    typedef std::vector<T> Container;

    Queue() : mu_(), cv_(&mu_) { }

    void Push(T x) {
        MutexLock l(&mu_);
        if (elems_.empty()) {
            cv_.Signal();
        }
        elems_.push_back(x);
    }

    bool FetchAll(Container* c, int wait_seconds) {
        MutexLock l(&mu_);
        while (elems_.empty()) {
            if (cv_.TimedWait(wait_seconds))
                return false;
        }
        c->swap(elems_);
        return true;
    }

  private:
    Mutex mu_;
    CondVar cv_;
    Container elems_;
};

} // namespace yohub

#endif // _YOHUB_JOBQUEUE_H_