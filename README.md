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
  
+ I included a example matrix multiplication in `ece6960-heterogeneous-computing/main/main.cpp`
+ Some benchmarks are as follows (with 16 workers):
  | matrix size | threadpool implementation runtime (us) | sequential implementation runtime (us) |
  | --- | --- | --- |
  | 256x256x256 | 3819 | 15673 | 
  | 512x512x512 | 50253 | 228273 |
  | 1024x1024x1024 | 636527 | 1914514 |
