#include <cuda.h>
#include <stdio.h>

#define K 2






__global__ void hello() {
	auto tid = threadIdx.x;
	auto bid = blockIdx.x;
	printf("tid=%u running from bid=%u\n", tid, bid);
}

int main() {
	hello<<< 1, 512 >>>();

	cudaDeviceSynchronize();


}
