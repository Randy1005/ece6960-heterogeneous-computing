#include "threadpool.hpp"


namespace tp {

// constructor tasks a unsigned integer representing the number of
// workers you need
Threadpool::Threadpool(size_t N) {
  // resize task queues
  // each worker gets a designated queue
  _task_queues.resize(N);
  

  for (size_t i = 0; i < N; i++) {
    threads.emplace_back([this, i, N]() {
      while (!stop) {
        std::function<void()> task;
        // iteratively grab a task from the queue
        {
          // Best practice: anything that happens inside the while continuation check
          // should always be protected by lock
          std::unique_lock lock(mtx);
          while(_task_queues[i].empty() && !stop) {
            cv.wait(lock);
          }
          if(!_task_queues[i].empty()) {
            task = _task_queues[i].front();
            _task_queues[i].pop();
          }
        }
        // and run the task...
        if(task) {
          task();
          
          // update which worker's turn to
          // be assigned a task
          if (_task_assign.load(std::memory_order_relaxed) < (N-1) ) {
            _task_assign.fetch_add(1, std::memory_order_relaxed);
          }
          else {
            _task_assign.store(0, std::memory_order_relaxed);
          }
        }

      } 
        
    });
  }
}

// destructor will release all threading resources by joining all of them
Threadpool::~Threadpool() {
  // I need to join the threads to release their resources
  for(auto& t : threads) {
    t.join();
  }
}

// shutdown the threadpool
void Threadpool::shutdown() {
  std::scoped_lock lock(mtx);
  stop = true;
  cv.notify_all();
}


} // ---------------------- end of namespace
