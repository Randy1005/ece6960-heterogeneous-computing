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


  template<typename Input, typename F>
  void for_each(Input beg, Input end, F func, size_t chunk_size = 1) {
    // get the total number of elements in the range
    size_t N = std::distance(beg, end);
    
    // define a token counter to 
    // indicate if we can still grab a chunk
    // from the range
    std::atomic<int> tokens{0};

    std::vector<std::future<void>> futures;
    for (size_t i = 0; i < threads.size(); i++) {
      futures.emplace_back(insert([&tokens, N, beg, end, func, chunk_size](){
        // calculate local begin / end
        size_t loc_beg = tokens.fetch_add(chunk_size);

        while (loc_beg < N) {
          // make sure we don't go out of bounds'
          size_t loc_end = std::min(loc_beg + chunk_size, N);
          
          // for this local range
          // apply function to each element
          std::for_each(beg+loc_beg, beg+loc_end, func);
          
          // grab the next chunk
          loc_beg = tokens.fetch_add(chunk_size);
        }

      }));

      // synchronize
      for (auto& fu : futures) {
        fu.get();
      }
    
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



void seq_foreach(std::vector<int>& vec) {
  std::foreach(vec.begin(), vec.end(), [](int& e) {
    e *= 10;
  })
  
}

void par_foreach(std::vector<int>& vec, Threadpool& tp) {
  tp.foreach(vec.begin(), vec.end(), [](int& e) {
    e *= 10; 
  }, 1024);
  
}

// ----------------------------------------
// Application Code
// ----------------------------------------
int main(int argc, char* argv[]) {

  if (argc != 3) {
    std::cerr << "usage ./a.out N T\n";
  }

  int N = std::atoi(argv[1]);
  int T = std::atoi(argv[2]);

  std::vector<int> v(N);
  // utilize whatever number of cores available
  Threadpool threadpool(T);
  std::vector<std::future<void>> futures;

  auto beg = std::chrono::steady_clock::now();
  seq_foreach()
  auto end = std::chrono::steady_clock::now();
   
  threadpool.shutdown(); 
  
  return 0;
}














