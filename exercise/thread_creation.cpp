#include <iostream>
#include <thread>
#include <vector>

// passing by pointer is more dangerous
// cuz pointer can be dangling
// we may be dereferencing a null pointer (illegal address)
// but in terms of performance
// using pointer and reference is the same
void func3(int* ref1) {
	std::cout << "func3's data = " << ref1 << "\n";
	*ref1 *= 3;
}

void func4(int& ref1) {
	ref1 *= 3;
}

void func1(int data) {
	std::cout << "func1's data = " << data << "\n";
}

void func2(int data) {
	std::cout << "func2's data = " << data << "\n";
}

void func5(int& data) {
	std::cout << "func5 executed by thread " << std::this_thread::get_id() << "\n";
	data *= 5;
	std::cout << "data = " << data << "\n";
}

int main(int argc, char* argv[]) {
	std::cout << "main thread id = " << std::this_thread::get_id() << "\n";

	std::vector<std::thread> threads;
	if (argc != 2) {
		std::cerr << "usage: ./a.out N\n";
		std::exit(EXIT_FAILURE);
	}

	int N = std::atoi(argv[1]);
	
	std::vector<int> data(N);
	for (int i = 0; i < N; i++) {
		data[i] = i;
	}

	// spawn N threads
	for (int i = 0; i < N; i++) {
		// according to cppref.com
		// emplace_back constructs an element in-place at the end
		// but push_back requires that thread to already be constructed
		//threads.emplace_back(func5);
		
		// if we wanna push_back
		// we have to use move semantics
		// cuz a thread is not copiable
		std::thread t(func5, std::ref(data[i]));
		threads.push_back(std::move(t));

		// emplace back is faster
		// cuz we don't need a temporary object'
	}

	// join these threads
	for (int i = 0; i < N; i++) {
		threads[i].join();
	}



}
