#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <numeric>
#include <cmath>
#include <sstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <mutex>
#include <time.h>
#include "Threadpool.h"
//process
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "err.h"

 
using namespace std;
 
void multmatseri(int **A, int **B, int **C, int N,int time);
void multmat(int **A, int **B, int **C, int N,int time);
 

int GetRandom(int max){
    srand(time(NULL));
    return rand() % max;
}

string GetTime(){
    auto nowtime = chrono::system_clock::now();
    time_t sleeptime = 
                chrono::system_clock::to_time_t(nowtime);
    return ctime(&sleeptime);
}

int main(int argcv, char * argv[]) {
 
    //ifstream label1 ("datos//datos.in"); // Abre el archivo de entrada de datos
    // Definición de variables y asignación dinámica de memoria
    srand (time(NULL));
    int i, j, k, N;
    int **A, **B, **C;
    int timeser,timethr;
    unsigned int No = thread::hardware_concurrency();
    
    N = stoi(argv[1]);
    cout << "Empiezo....: " << N;
    A = new int* [N], B = new int* [N], C = new int* [N];
    int segment_id;
    char* shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 0x6400;

    /* Allocate a shared memory segment. */
    segment_id = shmget (IPC_PRIVATE, shared_segment_size,
    IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    /* Attach the shared memory segment. */
    shared_memory = (char*) shmat (segment_id, 0, 0);
    printf ("shared memory attached at address %p\n", shared_memory);
    /* Determine the segment"s size. */
    shmctl (segment_id, IPC_STAT, &shmbuffer);
    segment_size = shmbuffer.shm_segsz;
    printf ("segment size: %d\n", segment_size);
    /* Write a string to the shared memory segment. */
    sprintf (shared_memory, "Hello, world.");
    /* Detach the shared memory segment. */
    shmdt (shared_memory);
    pid_t childrenhood;
    for (int g=0; g< No ;g++){
        childrenhood = fork();
        if (childrenhood != 0){
            cout << "i'm your motherfucking father" << getpid() << endl;
            break;

        }else{
            cout << "I'm the slut son " << getpid() << endl;
        }
        
    }

    for(j=0; j<N; j++){
        A[j] = new int [N], B[j] = new int [N], C[j] = new int [N];
    }   
 
    // llenado de las matrices (label1 apunta a datos.in en el subdirectorio datos del home    de usuario)
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            A[i][j] = rand() % 250;
        }
    }
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            B[i][j] = rand() % 150;        
        }
    }

    cout <<endl; cout <<endl; cout << endl;
    //multiplicacion con hilos
    multmat(A,B,C,N,timethr);       
    //Multiplicacion con serial
    multmatseri(A,B ,C,N,timeser);


   delete [] A, B, C; //Desasigna la memoria
 
   return 0;
   cout << "Termino.." << N;
 
}


// HILOS

void multmat(int **A, int **B, int **C, int N, int time){
    auto start = chrono::high_resolution_clock::now();
    int j, k;
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                C[i][j]=0;         
                for(int k=0; k<N; k++){

                    C[i][j]= C[i][j]+A[i][k]*B[k][j];
                }                                  

            }
        }
           
        
    
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "in Time of execute with thread "<< ": "<< duration.count() <<" microsegundos" <<endl <<endl;
    time = duration.count();
}

//serializacion

void multmatseri(int **A, int **B, int **C, int N,int time){
    auto start = chrono::high_resolution_clock::now();
    int i, j, k;
        for(i=0; i<N; i++){
            for(int j=0; j<N; j++){
                C[i][j]=0;         
                for(int k=0; k<N; k++){
                    C[i][j]= C[i][j]+A[i][k]*B[k][j];
                }
            }
        }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "in Time of execute without it "<< ": "<< duration.count() <<" microsegundos" <<endl <<endl;
    time = duration.count();
}





