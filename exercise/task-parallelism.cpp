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

template <typename T>
struct MoC {

  MoC(T&& rhs) : object(std::move(rhs)) {}
  MoC(const MoC& other) : object(std::move(other.object)) {}

  T& get() { return object; }

  mutable T object;
};

// ----------------------------------------
// Library Code
// ----------------------------------------
class Threadpool {
public:
  Threadpool(size_t n_threads) {
    for(int i=0; i<n_threads; i++) {
      threads.emplace_back([this](){
        // keep doing my job until the main thread sends a stop signal
        while(!stop) {
          std::function<void()> task;
          // my job is to iteratively grab a task from the queue
          {
            // Best practice: anything that happens inside the while continuation check
            // should always be protected by lock
            std::unique_lock lock(mtx);
            while(queue.empty() && !stop) {
              cv.wait(lock);
            }
            if(!queue.empty()) {
              task = queue.front();
              queue.pop();
            }
          }
          // and run the task...
          if(task) {
            task();
          }
        }
      });
    } 
  }


  // release all resources occupied by threads
  ~Threadpool() {
    for (auto& t : threads) {
      t.join();
    } 
  }

  void shutdown() {
    std::scoped_lock lock(mtx);
    stop = true;
    cv.notify_all();
  }

  // we know 'C' is a temporary variable
  // just in case some compilers are not
  // able to deduce const-reference type
  // and to avoid copying tasks
  //
  // we use perfect-forwarding
  // the compiler will forward whatever type that's
  // passed into the argument
  //
  // NOTE: this is not move-semantics
  template <typename C>
  auto insert(C&& task) {

    std::promise<void> promise;
    auto fu = promise.get_future();
    {
      std::scoped_lock lock(mtx);
      queue.push(
        [moc=MoC{std::move(promise)}, task=std::forward<C>(task)] () mutable {
          task();
          moc.object.set_value();
        }
      );
    }
    cv.notify_one();
    return fu;
  }

  template <typename C>
  auto insert_with_return(C&& task) {
    using R = std::result_of_t<C()>;
    std::promise<R> promise;
    auto fu = promise.get_future();
    {
      std::scoped_lock lock(mtx);
      queue.push(
        [moc=MoC{std::move(promise)}, task=std::forward<C>(task)] () mutable {
          // now the promise object carries the
          // return value of task()
          moc.object.set_value(
            task()
          );
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
  std::queue< std::function<void()> > queue;

};




// ----------------------------------------
// Application Code
// ----------------------------------------
int main(int argc, char* argv[]) {

  // utilize whatever number of cores available
  Threadpool threadpool(std::thread::hardware_concurrency());

  // say we wanna create a task dependency graph
  // A -> B
  // A -> C
  // B -> D
  // C -> D
  
  std::future<void> fu_A = threadpool.insert([]() {
    std::cout << "running task A\n";
  });

  // we cannot get fu_A twice
  // futures by default can only be 
  // carried out once

  // therefore we need to migrate fu_A
  // to a shared state
  auto shared_fu_A = fu_A.share();

  std::future<void> fu_B = threadpool.insert([&]() {
    shared_fu_A.get();
    std::cout << "running task B\n";
  });


  std::future<void> fu_C = threadpool.insert([&]() {
    shared_fu_A.get();
    std::cout << "running task C\n";
  });
  
  
  std::future<void> fu_D = threadpool.insert([&]() {
    fu_B.get();
    fu_C.get();
    std::cout << "running task D\n";
  });
 
  fu_D.get();

  threadpool.shutdown(); 
  
  return 0;
}














