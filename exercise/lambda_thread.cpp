#include <iostream>
#include <thread>

int main() {
	int data = 10;

	// if we're capturing by value
	// we need to specify mutable
	// cuz lambda functions are default const
	// captured data are read-only
	std::thread t1([data] () mutable {
		data *= 20;
		std::cout << "data is " << data << ", by t1 " << std::this_thread::get_id() << "\n";
	});




	t1.join();

	std::cout << "data is " << data << ", by main thread " << std::this_thread::get_id() << "\n";
}
