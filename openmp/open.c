#include <stdio.h>
#include <omp.h>
#include <time.h>





void simple(int n, float *a, float *b){    
    int i;
    clock_t start, end;
    start = clock(); 
    #pragma omp parallel for     
    for (i=1; i<n; i++) {
    /* i is private by default */        
    b[i] = (a[i] + a[i-1]) / 2.0;
    }
    end = clock()-start;
    double time_taken = ((double)end)/CLOCKS_PER_SEC;
    printf("with openMP %f\n",time_taken);
}

void whsimple(int n, float *a, float *b){
    int i;
    clock_t start, end;
    start = clock(); 

    for (i=1; i<n; i++) {
    /* i is private by default */        
    b[i] = (a[i] + a[i-1]) / 2.0;
    }
    end = clock()-start;
    double time_taken = ((double)end)/CLOCKS_PER_SEC;
    printf("without oneMP %f\n",time_taken);
    
}



int main(int argcv, char * argv[]){
    int n = 5;
    float A[] = {1.0,1.0,1.0,1.0,1.0}; 
    float B[] = {1.0,1.0,1.0,1.0,1.0};

    simple(n,A,B);
    whsimple(n,A,B);
    return 0;
}