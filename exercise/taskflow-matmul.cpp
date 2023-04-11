#include <taskflow/taskflow.hpp>

void matmul(
  int N,
  int K,
  int M,
  const std::vector<int>& A,
  const std::vector<int>& B,
  std::vector<int>& C,
  tf::Executor& executor
  ) {
  tf::Taskflow taskflow;

  // use the parallel for primitive
  taskflow.for_each_index(0, static_cast<int>(N), 1,
    [=, &A, &B, &C](int i) {
      for (int j = 0; j < M; j++) {
        for (int k = 0; k < K; k++) {
          C[i * M + j] += A[i * K + k] * B[k * M + j];
        }
      }
    }
  );

  executor.run(taskflow).wait();

  taskflow.dump(std::cout);
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
    std::exit(EXIT_FAILURE);
  }

  size_t N = std::atoi(argv[1]);
  size_t K = std::atoi(argv[2]);
  size_t M = std::atoi(argv[3]);
  size_t T = std::atoi(argv[4]);

  std::vector<int> A(N*K, 1), B(K*M, 2), C(N*M, 0);

  // input num of threads from argc
  tf::Executor executor(T);

  auto beg = std::chrono::steady_clock::now();
  matmul(N, K, M, A, B, C, executor);
  auto end = std::chrono::steady_clock::now();

  std::cout << "matmul runtime = "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                end - beg
               ).count()
            << " us\n";

   

  return 0;
}














