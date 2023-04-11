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



void matmul(
  int N,
  int K,
  int M,
  const std::vector<int>& A,
  const std::vector<int>& B,
  std::vector<int>& C,
  Threadpool& pool
  ) {
  
  // sequential version matmul
  /*
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      for (int k = 0; k < K; k++) {
        C[i * M + j] += A[i * K + k] * B[k * M + j];
      }
    }
  }
  */

  std::vector<std::future<void>> futures;

  // parallelized version (issue of false sharing)
  /*
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      futures.emplace_back(
        pool.insert([M, K, i, j, &A, &B, &C]() {
          for (int k = 0; k < K; k++) {
            C[i * M + j] += A[i * K + k] * B[k * M + j];
          }
        })
      );
    }
  }
  */
  for (int i = 0; i < N; i++) {
    futures.emplace_back(
      // NOTE: capture everything by value
      // except A, B, C
      pool.insert([=, &A, &B, &C]() {
        for (int j = 0; j < M; j++) {
          for (int k = 0; k < K; k++) {
            C[i * M + j] += A[i * K + k] * B[k * M + j];
          }
        }
      })
    );
  }

  // synchornize
  for (auto& fu : futures) {
    fu.get();
  }
}


// ----------------------------------------
// Application Code
//
//
// Matrix Multiplication
//
// A * B = C
//
// A: NxK
// B: KxM
// C: NxM
// ----------------------------------------
int main(int argc, char* argv[]) {

  if (argc != 5) {
    std::cerr << "usage: ./a.out N K M T\n";
  }

  size_t N = std::atoi(argv[1]);
  size_t K = std::atoi(argv[2]);
  size_t M = std::atoi(argv[3]);
  size_t T = std::atoi(argv[4]);

  std::vector<int> A(N*K, 1), B(K*M, 2), C(N*M, 0);

  // input num of threads from argc
  Threadpool threadpool(T);

  auto beg = std::chrono::steady_clock::now();
  matmul(N, K, M, A, B, C, threadpool);
  auto end = std::chrono::steady_clock::now();

  
  threadpool.shutdown(); 
 
  std::cout << "matmul runtime = "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                end - beg
               ).count()
            << " us\n";

  return 0;
}














