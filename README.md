# ece6960-heterogeneous-computing
ece6960-heterogeneous-computing assigments and materials


## Programming Assignment 1 Instructions
+ Compile and Run:
  + Clone the repo
  + Change directory to PA1: `cd ece6960-heterogeneous-computing/assignments/PA1/`
  + `mkdir build && cd build`
  + `cmake ..`
  + run `make` to build executable
  + executable named `main` will be inside the `ece6960-heterogeneous-computing/build/main/` folder

+ Modifications
  + I implemented an individual task queue for each thread, and distributed tasks by letting the queues take turns inserting tasks 
  
+ I included an example matrix multiplication in `ece6960-heterogeneous-computing/main/main.cpp`
+ Some benchmarks are as follows (with 16 workers):
  | matrix size | threadpool implementation runtime (us) | sequential implementation runtime (us) | speedup |
  | --- | --- | --- | --- |
  | 256x256x256 | 3819 | 15673 | 4.1x |
  | 512x512x512 | 50253 | 228273 | 4.5x | 
  | 1024x1024x1024 | 636527 | 1914514 | 3x |


## Programming Assignment 2 Instructions
+ Compile and Run:
  + Clone the repo
  + Change directory to PA2: `cd ece6960-heterogeneous-computing/assignments/PA2/`
  + Compile `parallel-reduction.cpp` with `g++ parallel-reduction.cpp -lpthread -std=c++17`
  + executable usage: `./a.out [num-threads] [data-size]`, e.g. `./a.out 8 10000000`
+ The parallel guided reduction function is named `reduce_guided`
+ Some benchmarks are as follows (16 workers):
| data size | parallel guided reduce runtime (ns) | seqential reduce runtime (ns) | speedup |
| --- | --- | --- | --- |
| 100000 | 382144 | 689356 | 1.8x |
| 1000000 | 1042555 | 6940653 | 6.6x |
| 10000000 | 8331411 | 72429036 | 8.7x|
| 100000000 | 80238743 | 709533506 | 8.8x|


