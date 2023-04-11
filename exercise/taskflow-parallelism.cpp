#include <taskflow/taskflow.hpp>

int main() {
	tf::Taskflow taskflow;
	tf::Executor executor(10);

	auto A = taskflow.emplace([]() {
		std::cout << "Task A\n";
	});


	auto B = taskflow.emplace([]() {
		std::cout << "Task B\n";
	});


	auto C = taskflow.emplace([]() {
		std::cout << "Task C\n";
	});


	auto D = taskflow.emplace([]() {
		std::cout << "Task D\n";
	});

	A.precede(B, C);
	B.precede(D);
	C.precede(D);

	auto fu = executor.run(taskflow);
	fu.get();
}
