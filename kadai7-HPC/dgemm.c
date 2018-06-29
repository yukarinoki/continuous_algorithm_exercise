#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <math.h> 
#include <sys/time.h>
#include "mkl.h"
#define IDX(mat,n, i,j) (mat)[(i)+(j)*(n)] 

double get_dtime(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec + (double)(tv.tv_usec)*0.001*0.001;
}

void run(int SIZE){ double t1,t2;
  // c[1:M][1:N] = a[1:M][1:K] * b[1:K][1:N] double *a,*b,*c;
  int m=SIZE, n=SIZE, k=SIZE;
  int SQ=SIZE*SIZE;
  a = (double*)malloc(sizeof(double)*SQ); b = (double*)malloc(sizeof(double)*SQ); c = (double*)malloc(sizeof(double)*SQ);
  for(int i=0; i<m; i++){ for(int j=0; j<k; j++) IDX(a, m, i, j) = uniform(); 
    for(int i=0; i<k; i++) for(int j=0; j<n; j++) IDX(b, k, i, j) = uniform();
    double alpha=1.0, zero=0.0; 
    /*
      printf("matrix A:\n"); print_matrix(a, m, k); printf("matrix B:\n"); print_matrix(b, k, n);
    */
    t1 = get_dtime();
    dgemm("N","N", &m,&n,&k, &alpha,a,&m, b,&k, &zero,c,&m); t2 = get_dtime();
    /*
      printf("matrix C:\n");
      print_matrix(c, m, n);
    */
    printf("%d,%.10e\n", SIZE,t2-t1);
    free(c);
    free(b);
    free(a);
  }
  int main(){
    for (int i=4100; i<=7000;i+=100){ run(i);
    }
    return 0;
  }
