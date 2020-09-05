#include <stdio.h>
#include <omp.h>
#include <time.h>


int main(){  
    int x; x = 2;
    clock_t start, end;
    start = clock();  

    #pragma omp parallel num_threads(2) shared(x)
    {    if (omp_get_thread_num() == 0) {       
        x = 5;    
    } else {     
        /* Print 1: the following read of x has a race */       
        printf("1: Thread# %d: x = %d\n", omp_get_thread_num(),x );     
        }
      
    #pragma omp barrier    
    if (omp_get_thread_num() == 0) {    
        /* Print 2 */       
        printf("2: Thread# %d: x = %d\n", omp_get_thread_num(),x );    
        } else {    
            /* Print 3 */       
            printf("3: Thread# %d: x = %d\n", omp_get_thread_num(),x );    
                }  
            }
    end = clock()-start;
    double time_taken = ((double)end)/CLOCKS_PER_SEC;
    printf("with openMP  %f\n",time_taken);

    /* La directiva parallel forma un equipo de hebras y inicia la ejecución paralela y 
    La contrucción barrier especifica de forma explicita un punto de sincronización para todas las hebras.
    */

    return 0;
}