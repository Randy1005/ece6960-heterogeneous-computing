#include <iostream>
#include <threadpool/threadpool.hpp>
#include <chrono>
#include <fstream>

int main(int argc, char* argv[]) {
 
  tp::Threadpool threadpool(5);
  
  std::vector<std::future<void>> futures;
  for (size_t i = 0; i < 1000; i++) {
    futures.emplace_back(threadpool.insert([i]() {
      std::cout << "Task " << i << "\n";  
    }));
  }

  for (auto& fu : futures) {
    fu.get();
  }

  threadpool.shutdown();

  return 0;
}
