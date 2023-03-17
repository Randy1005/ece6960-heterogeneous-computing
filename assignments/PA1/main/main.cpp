#include <iostream>
#include <threadpool/threadpool.hpp>
#include <chrono>
#include <fstream>

void matmul_seq(
  size_t N, size_t K, size_t M,
  const std::vector<int>& A,
  const std::vector<int>& B,
  std::vector<int>& C
) {
  // seq version of matmul
  for(size_t i=0; i<N; i++) {
    for(size_t j=0; j<M; j++) {
      C[i*M + j] = 0;
      for(size_t k=0; k<K; k++) {
        C[i*M + j] += A[i*K + k] * B[k*M + j];
      }
    }
  }
}

void matmul(
  size_t N, size_t K, size_t M,
  const std::vector<int>& A,
  const std::vector<int>& B,
  std::vector<int>& C,
  tp::Threadpool& threadpool
) {

  std::vector<std::future<void>> futures;
  
  for(size_t i=0; i<N; i++) {
    futures.emplace_back(
      threadpool.insert([=, &A, &B, &C](){
        for(size_t j=0; j<M; j++) {
          // each element C[i][j] is the result of inner product
          // of row i at A and column j at B
          for(size_t k=0; k<K; k++) {
            C[i*M + j] += A[i*K + k] * B[k*M + j];
          }
        }
      })
    );
  }
  
  // synchronize the execution on the N*M inner products
  for(auto& fu : futures) {
    fu.get();
  }
}

int main(int argc, char* argv[]) {

  if(argc != 5) {
    std::cerr << "usage: ./a.out N K M T\n";
    std::exit(EXIT_FAILURE);
  }

  size_t N = std::atoi(argv[1]);
  size_t K = std::atoi(argv[2]);
  size_t M = std::atoi(argv[3]);
  size_t T = std::atoi(argv[4]);  // number of threads to parallelize
                                  // the matrix multiplication

  // initialize three matrices A, B, and C
  std::vector<int> A(N*K, 1), B(K*M, 2), C(N*M, 0);

  // create a thread pool of the maximum hardware concurrency
  tp::Threadpool threadpool(T);

  // run matrix multiplication in parallel
  auto beg = std::chrono::steady_clock::now();
  matmul(N, K, M, A, B, C, threadpool);
  auto end = std::chrono::steady_clock::now();

  std::cout << "Parallel AxB=C takes "
            << std::chrono::duration_cast<std::chrono::microseconds>(end-beg).count()
            << " us\n";

  // run matrix multiplication in sequential
  beg = std::chrono::steady_clock::now();
  matmul_seq(N, K, M, A, B, C);
  end = std::chrono::steady_clock::now();

  std::cout << "Sequential AxB=C takes "
            << std::chrono::duration_cast<std::chrono::microseconds>(end-beg).count()
            << " us\n";
  
  // shut down the threadpool
  threadpool.shutdown();


  return 0;
}






