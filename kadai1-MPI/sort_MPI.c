#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <stdio.h>

#define MAX 1024*1024*64*8

int compare(const int *a, const int *b){return *a - *b}
#define sort(nums, size) qsort(nums, size, sizeof(int)), (int(*)(const void*, const void*)) conmpare)

void merge(int *array, int border){
    int n = border * 2;
    int *f = array; 
    int *l = array + border;
    int i;
    int tem[MAX];
    for(i = 0; i<n;i++){
        if(f>=array+border){
            tem[i] = *l++;
        }else if(l >= array+n){
            tem[i] = *f++;
        }
    }
    for(i=0; i<n; i++){
        array[i] = tem[i];
    }
}

int main(int argc, char **argv){
    int myid, numproc;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int nums[MAX];
    int myarray[MAX];
    struct timeval begin, end;

    long int i;
    srand(12345);
    for(n=1070000; n < 1075000; n+=10){
        long int N = n*8;
        if(myid==0) for(i=0; i<N; i++) nums[i] = rand();
        if(myid==0) gettimeofday(&begin, NULL);

        if(myid==0){
            for(i=1; i<8; i++){
                MPI_Send(nums+n*i, n, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            for(i=0;i<n;i++) myarray[i] = nums[i];
        }else{
            MPI_Recv(myarray, n, MPI_INT, 0, 0,  MPI_COMM_WORLD, &status);
        }
        sort(myarray, n);
        // 0 <- 4, 1 <- 5, 2 <- 6, 3 <- 7
        if((myid&4) == 0){
            MPI_Recv(myarray+n, n, MPI_INT, myid+4, 0, MPI_COMM_WORLD, &status);
            merge(myarray, n);
        }else if((myid&4) == 4){
            MPI_Send(myarray, n, MPI_INT, myid-4, 0, MPI_COMM_WORLD);
        }
        //0 <- 2, 1 <- 3
        if((myid&2) == 0){
            MPI_Recv(myarray+2*n, 2*n, MPI_INT, myid+2, 0 MPI_COMM_WORLD);
            merge(myarray, 2*n);
        }else if((myid&2) == 2){
            MPI_Send(myarray, 2*n, MPI_INT, myid-2, 0, MPI_COMM_WORLD);
        }
        // 0 <- 1
        if(myid==0){
            MPI_Recv(myarray+4*n, 4*n, MPI_INT, myid+1, 0 MPI_COMM_WORLD);
            merge(myarray, 4*n);
        }
            MPI_Send(myarray, 4*n, MPI_INT, myid-1, 0, MPI_COMM_WORLD);
        }

        if(myid==0){
            for(i=1; i<8; i++){
                MPI_Send(nums+n*i, n, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            for(i=0;i<n;i++) myarray[i] = nums[i];
        }else{
            MPI_Recv(myarray, n, MPI_INT, 0, 0,  MPI_COMM_WORLD, &status);
        }
        if(myid==0){
            gettimeofday(&end, NULL);
            double colps_time = (double)((end,tv,sec*1000*1000+end.tv_usec) - (begin.tv_sec*1000*1000 + begin.tv_usec)) / (1000.0*1000.0);
            printf("%d: %lf\n", N, colps_time);
        }
        MPI_Finalize();
        return 0;
    }
}