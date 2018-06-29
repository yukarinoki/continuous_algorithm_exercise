#include <stdio.h> 
#include <sys/time.h> 
#include <stdlib.h>
#define ITER 100
#define SIZE 128
#define R 0.1
int main(){
  float u[SIZE][SIZE][2];
  double t,c;
  struct timeval t0, t1;
  for(int t=0;t<2;t++){
      for(int i=0;i<SIZE;i++){
          for(int j=0;j<SIZE;j++){
              u[i][j][t]= (i==0||j==0||i==SIZE-1||j==SIZE-1) ? 0 : 1;
          } 
      }
  }
  gettimeofday(&t0, NULL);
  for(int t=1;t<ITER+1;t++){
   for(int i=1;i<SIZE-1;i++){
     for(int j=1;j<SIZE-1;j++){
    u[i][j][t%2] = 
        (1 - 4*R)*u[i][j][(t-1)%2]
    +   R * (u[i+1][j][(t-1)%2]
    +   u[i][j+1][(t-1)%2]
    +   u[i-1][j][(t-1)%2]
    +   u[i][j-1][(t-1)%2]);
} 
}
}
  gettimeofday(&t1, NULL);
  for(int i=1;i<SIZE-1;i++){
   for(int j=1;j<SIZE-1;j++){
     c+=u[i][j][0];
} }
  t = (double)(t1.tv_sec - t0.tv_sec) + (double)(t1.tv_usec - t0.tv_usec) * 0.000001;
printf("Time = %lf, FLOPS = %lf, Check_sum = %lf¥n", t, (double)6*ITER*SIZE*SIZE/t,c);
return 0; 
}
