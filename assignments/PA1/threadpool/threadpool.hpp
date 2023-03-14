#include <iostream>
#include <chrono>
#include <vector>
#include <future>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <type_traits>
#include <atomic>

namespace tp {
template <typename T>
struct MoC {

  MoC(T&& rhs) : object(std::move(rhs)) {}
  MoC(const MoC& other) : object(std::move(other.object)) {}

  T& get() { return object; }

  mutable T object;
};


class Threadpool {
public:
  // constructor tasks a unsigned integer representing the number of
  // workers you need
  Threadpool(size_t N);

  // destructor will release all threading resources by joining all of them
  ~Threadpool();

  // shutdown the threadpool
  void shutdown();


  // insert a task "callable object" into the threadpool
  // NOTE: template methods cannot be put in separate files?
  // causes 'auto' type unable to be deduced
  template <typename C>
  auto insert(C&& task) {
    std::promise<void> promise;
    auto fu = promise.get_future();
    {
      std::scoped_lock lock(mtx);
      _task_queues[_task_assign.load()].push(
        [moc=MoC{std::move(promise)}, task=std::forward<C>(task)] () mutable {
          task();
          moc.object.set_value();
        }
      );
    }

    // NOTE: I have no control over which thread to wake up
    // so this has to be notify_all
    // better way to do this?
    cv.notify_all();
    return fu;
  }


  // insert a task "callable object" into the threadpool
  template <typename C>
  auto insert_with_return(C&& task) {
    using R = std::result_of_t<C()>;
    std::promise<R> promise;
    auto fu = promise.get_future();
    {
      std::scoped_lock lock(mtx);
      queue.push(
        [moc=MoC{std::move(promise)}, task=std::forward<C>(task)] () mutable {
          moc.object.set_value(
            task()
          );
        }
      );
    }
    cv.notify_one();
    return fu;
  }

  // insert a task "callable object" into the threadpool using a generic
  // function wrapper (instead of a template argument)
  auto insert_2(std::function<void()> task) {
    std::promise<void> promise;
    auto fu = promise.get_future();

    {
      std::scoped_lock lock(mtx);
      queue.push(
        [moc=MoC{std::move(promise)}, task=std::move(task)] () mutable {
          task();
          moc.object.set_value();
        }
      );
    }
    cv.notify_one();
    
    return fu;
  }


private:
  std::mutex mtx;
  std::vector<std::thread> threads;
  std::condition_variable cv;
  bool stop {false};
  
  std::vector<std::queue< std::function<void() >>> _task_queues;
  std::queue<std::function<void()>> queue;

  // records which task queue to 
  // store a task
  std::atomic<int> _task_assign = {0};

};

}

