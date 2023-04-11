#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> cnt = {0};



void f() {
  for (int i = 0; i < 1000; i++) {
    // setting strict sequential consistency
    // cnt.fetch_add(1, std::memory_order::seq_cst); // same as cnt++
    
    // ^ but this is very strict, we're basically
    // telling the compiler NOT to reorder anything before / after
    // cnt++
    // we want the compiler to still have the flexibility
    // to reorder instructions before / after cnt++
    // while respecting the atomic constraint
    cnt.fetch_add(1, std::memory_order::relaxed);
  }

}

int main() {
  std::vector<std::thread> v;
  for (int n = 0; n < 10; n++) {
    v.emplace_back(f);
  }

  for (auto& t : v) {
    t.join();
  }
  
  std::cout << "final cnt value = " << cnt << "\n";
}
