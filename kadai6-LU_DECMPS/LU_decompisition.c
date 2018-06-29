#include <stdio.h>
#include <stdlib.h>

#define N 6
//--- matrix access ----//
// mat[i, j]
//rowsize   : n
//row       : i
//column    : j
#define IDX(mat,n,i,j) (mat)[(i)+(j)*(n)]
//----------------------//

void print_matrix(double *a, int m){
    for(int i=0; i<m; i++){
        for(int j=0; j<m; j++){
            printf("%.4f ", IDX(a,m,i,j));
        }
    printf("\n");
    }
}

double fabs(double d){
    return d < 0 ? -d : d;
}

int lu_decomposition(double *a, double *l, double *u, int m, int *piv){
    if(m==1){
        IDX(l,m,0,0) = 1.0;
        IDX(u,m,0,0) = IDX(a,m,0,0);
        *piv=0;
        return 0;
    }

    //partial_pivoting ;
    double max_a = 0;
    *piv=0;
    for(int i=0;i<m;i++){
        if(fabs(IDX(a,m,i,0)) > max_a){
            max_a = fabs(IDX(a,m,i,0));
            *piv = i;
        }
    }

    for(int i=0;i<m;i++){
        double tem;
        tem = IDX(a,m,0,i);
        IDX(a,m,0,i) = IDX(a,m,*piv,i);
        IDX(a,m,*piv,i) = tem;
    }

    IDX(u,m,0,0) = IDX(a,m,0,0);
    for(int i=1; i<m; i++) IDX(u,m,i,0) = 0.0;
    for(int i=1; i<m; i++) IDX(u,m,0,i) = IDX(a,m,0,i);

    IDX(l,m,0,0) = 1.0;
    for(int i=1; i<m; i++) IDX(l,m,i,0) = IDX(a,m,i,0) / IDX(u,m,0,0);

    int size = (m-1) * (m-1);
    double *sub_a,*sub_l,*sub_u;
    sub_a = (double *) malloc(sizeof(double)*size);
    sub_l = (double *) malloc(sizeof(double)*size);
    sub_u = (double *) malloc(sizeof(double)*size);

    for(int i=1;i<m;i++) for(int j=1;j<m;j++)
            IDX(sub_a,m-1,i-1,j-1) = IDX(a,m,i,j) - IDX(l,m,i,j) - IDX(u,m,0,j);

    lu_decomposition(sub_a,sub_l,sub_u,m-1,piv+1);

     for(int i=1;i<m;i++) for(int j=1;j<m;j++){
         IDX(l,m,i,j) = IDX(sub_l,m-1,i-1,j-1);
         IDX(u,m,i,j) = IDX(sub_u,m-1,i-1,j-1);
     }

     for(int i=1; i<m;i++){
         if(piv[i] != 0){
             double tem = IDX(l,m,i,0);
             IDX(l,m,i,0) = IDX(l,m,i+piv[i],0);
             IDX(l,m,i+piv[i],0) = tem;
         }
     }

     free(sub_a);
     free(sub_u);
     free(sub_l);
     return 0;
}

int where_a(int idx, int *piv,int m){
    for(int i=0;i<m;i++){
        if(piv[i]==idx) return i;
        else if(idx==0) idx = piv[i];
        idx--;
    }
    // doesn't reach here
    return m;
}

int main(){
    double *a, *l, *u, *lu, *pa;
    int *piv;
    int size = N*N;
    piv = (int *)malloc(sizeof(int)*N);
    a = (double *)malloc(sizeof(int)*size);
    u = (double *)malloc(sizeof(int)*size);
    l = (double *)malloc(sizeof(int)*size);
    lu = (double *)malloc(sizeof(int)*size);
    pa = (double *)malloc(sizeof(int)*size);

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            IDX(a,N,i,j) = 1.0 / (double)(1 + i + j);
        }
    }
    lu_decomposition(a,l,u,N,piv);

    //construct_P

    double *p;
    p = (double *)calloc(size, sizeof(double));
    for(int i=0; i<N; i++){
        IDX(p,N,where_a(i,piv,N), i) = 1;
    }

    printf("\n");

    //compute PA
    for(int i=0; i<N;i++){
        for(int j=0;j<N;j++){
            IDX(pa,N,i,j) = 0;
            for(int k=0;k<N;k++){
                IDX(pa,N,i,j) += IDX(p,N,i,k) * IDX(a,N,k,j);
            }
        }
    }

    //compute L \times U
    for(int i=0; i<N;i++){
        for(int j=0;j<N;j++){
            IDX(lu,N,i,j) = 0;
            for(int k=0;k<N;k++){
                IDX(lu,N,i,j) += IDX(l,N,i,k) * IDX(u,N,k,j);
            }
        }
    }

    printf("P=\n");
    print_matrix(p,N);
    printf("A=\n");
    print_matrix(a,N);
    printf("L=\n");
    print_matrix(l,N);
    printf("U=\n");
    print_matrix(u,N);
    printf("P time A = \n");
    print_matrix(pa,N);
    printf("L times U =\n");
    print_matrix(lu, N);

    return 0;
}