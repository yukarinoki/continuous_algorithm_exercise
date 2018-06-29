#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <math.h> 
#include <sys/time.h> 
#include <omp.h>
#define IDX(mat,n, i,j) (mat)[(i)+(j)*(n)] 
//-------------------------//
void print_matrix(double *a, int m, int n){ 
  for(int i=0; i<m; i++){
    for(int j=0; j<n; j++) printf("%.2f ", IDX(a,m, i,j));
    printf("\n"); 
  }
}
double uniform(){
  static int init_flg = 0; 
  if(!init_flg){
    init_flg = 1;
    srand((unsigned)time(NULL)); 
  }
  return ((double)rand()+1.0)/((double)RAND_MAX+2.0); 
}
double get_dtime(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec + (double)(tv.tv_usec)*0.001*0.001;
}
void run(int SIZE){
  double t1,t2;
  double *a,*b,*c;
  int m=SIZE, n=SIZE, k=SIZE;
  int SQ=SIZE*SIZE;
  a = (double*)malloc(sizeof(double)*SQ); b = (double*)malloc(sizeof(double)*SQ); c = (double*)malloc(sizeof(double)*SQ);
  for(int i=0; i<m; i++) for(int j=0; j<k; j++)
			   IDX(a, m, i, j) = uniform(); 
  for(int i=0; i<k; i++) for(int j=0; j<n; j++)
			   IDX(b, k, i, j) = uniform();
  for(int i=0; i<k; i++) for(int j=0; j<n; j++)
			   IDX(c, k, i, j) = 0.0; 
    
  double alpha=1.0, zero=0.0;
  t1 = get_dtime();

#pragma omp parallel for
  for(int i=0;i<m;i++) for(int j=0;j<m;j++) for(int l=0;l<m;l++) 
					      IDX(c, k, i, j) += IDX(a, k, i, l) * IDX(b, k, l, j);
  
  t2 = get_dtime(); printf("%d,%.10e\n", SIZE,t2-t1);
  free(c);
  free(b);
  free(a);
}
int main(){
  for (int i=100; i<=4000;i+=100){ run(i);
  }
  return 0;
}
