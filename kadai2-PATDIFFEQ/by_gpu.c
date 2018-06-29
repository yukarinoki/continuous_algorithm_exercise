
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cuda.h>
#include <cuda_runtime.h>

#define ITER 100
#define SIZE 128
const float r = 0.1;
__global__ void Sub(float *from, float *to) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int y = index / SIZE, x = index % SIZE;
    if (x==0||x==SIZE-1||y==0||y==SIZE-1) {
        to[index] = 0.0;
    } else {
        to[index] = (1-4*r)*from[index]+r*(from[index-
1]+from[index+1]+from[index-SIZE]+from[index+SIZE]);
} }
int main() {
    struct timeval t0, t1;
    double t,c;
    float *data;
    float *bufA, *bufB, *buftmp;
    int i, j;
    dim3 Dg(SIZE * SIZE / 512, 1, 1);
    dim3 Db(512, 1, 1);
    data = (float*) malloc(sizeof(float) * SIZE * SIZE);
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            if (i == 0||i==SIZE-1||j==0||j==SIZE-1) {
                data[i*SIZE+j] = 0.0;
            } else {

                data[i*SIZE+j] = 1.0;
            }
} }
    cudaMalloc(&bufA, sizeof(float) * SIZE * SIZE);
    cudaMalloc(&bufB, sizeof(float) * SIZE * SIZE);
    cudaMemcpy(bufA, data, sizeof(float) * SIZE * SIZE,
cudaMemcpyHostToDevice);
    gettimeofday(&t0, NULL);
    for (i = 0; i < ITER; ++i) {
        Sub<<<Dg, Db>>>(bufA, bufB);
        cudaThreadSynchronize();
        buftmp = bufA;
        bufA = bufB;
        bufB = buftmp;
}
    cudaMemcpy(data, bufA, sizeof(float) * SIZE * SIZE,
cudaMemcpyDeviceToHost);
    cudaThreadSynchronize();
    gettimeofday(&t1, NULL);
    for(i=0;i<SIZE*SIZE;i++){
    c += data[i];
}
    t = (double)(t1.tv_sec - t0.tv_sec) +
(double)(t1.tv_usec - t0.tv_usec) * 0.000001;
printf("Time = %lf, FLOPS = %lf, Checksum = %lf¥n", t, (double)6*ITER*SIZE*SIZE/t, c);
    cudaFree(bufA);
    cudaFree(bufB);

free(data);
return 0; 
}