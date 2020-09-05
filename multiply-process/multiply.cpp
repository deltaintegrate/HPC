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
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "err.h"

#define No  std::thread::hardware_concurrency()

 
using namespace std;
 
void multmatseri(int **A, int **B, int **C,int N,int time);
int multmat(int **A, int **B, int **C, int N,int time, pid_t childrenhood);
 

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
    int i, j, k, N,count;
    int **A, **B, **C,**F;
    int **At, **Bt, **Ct;
    int timeser,timethr;
    //unsigned int No = thread::hardware_concurrency();
    
    N = stoi(argv[1]);
    //cout << "Empiezo....: " << N <<endl;
    A = new int* [N], B = new int* [N], C = new int* [N], F = new int* [N];
    At = new int* [N], Bt = new int* [N], Ct = new int* [N];
    int segment_id;
    char* shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 0x6400;

    /* Allocate a shared memory segment. */
    segment_id = shmget (IPC_PRIVATE, shared_segment_size,
    IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    /* Attach the shared memory segment. */
    //shared_memory = (char*) shmat (segment_id, (void*)0x5000000, 0);
    A = (int **) shmat(segment_id,(void*)0x6000000, 0);
    B = (int **) shmat(segment_id,(void*)0x7000000, 0);
    C = (int **) shmat(segment_id,(void*)0x8000000, 0);
    //counter of process
    At = (int **) shmat(segment_id,(void*)0x3000000, 0);
    Bt = (int **) shmat(segment_id,(void*)0x4000000, 0);
    Ct = (int **) shmat(segment_id,(void*)0x5000000, 0);
    //printf ("shared memory attached at address %p\n", shared_memory);
    /* Determine the segment"s size. */
    shmctl (segment_id, IPC_STAT, &shmbuffer);
    segment_size = shmbuffer.shm_segsz;
    //printf ("segment size: %d\n", segment_size);
    /* Write a string to the shared memory segment. */
    //sprintf (shared_memory, "Hello, world.");
    /* Detach the shared memory segment. */
    //shmdt (shared_memory);
    /*
    pid_t childrenhoo
    for (int g=0; g< No ;g++){
        childrenhood = fork();
        if (childrenhood != 0){
            cout << "i'm your motherfucking father" << getpid() << endl;
            
            break;

        }else{
            cout << "I'm the slut son " << getpid() << endl;
        }
        
    }
    */
    for(j=0; j<N; j++){
        A[j] = new int [N], B[j] = new int [N], C[j] = new int [N];
    }
    for(j=0; j<N; j++){
        At[j] = new int [N], Bt[j] = new int [N], Ct[j] = new int [N];
    }   
 
    // llenado de las matrices (label1 apunta a datos.in en el subdirectorio datos del home    de usuario)
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            At[i][j] = rand() % 250;
        }
    }
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            Bt[i][j] = rand() % 150;        
        }
    }
    //transponer Matrix A y B

    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            A[j][i] = At[i][j];
        }
    }
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            B[j][i] = Bt[i][j];        
        }
    }

    //cout <<endl; cout <<endl; cout << endl;
    pid_t childrenhood; 
    //multiplicacion con hilos
    multmat(A,B,C,N,timethr,childrenhood);       
    //Multiplicacion con serial
    multmatseri(A,B ,C,N,timeser);
    shmdt (A);
    shmdt (B);
    shmdt (C);
    //delete [] A, B, C; //Desasigna la memoria
 
   return 0;
   cout << "Termino.." << N;
 
}


// procesos

int multmat(int **A, int **B, int **C, int N, int time, pid_t childrenhood){
    auto start = chrono::high_resolution_clock::now();
    int j, k;
        for(int i=0; i<N; i++){
            pid_t childrenhood;
            childrenhood = fork();
            if( childrenhood){
                for(int j=0; j<N; j++){
                C[i][j]=0;
                for(int k=0; k<N; k++){
                    C[i][j]= C[i][j]+A[k][i]*B[j][k];
                        }
                    }
            //break;        
            exit(1); 
            }else continue;

            }
    
    for (int dis=0; dis <N;dis++)wait (&childrenhood);

    
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() <<",";
    time = duration.count();

    return 0;
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
    cout << duration.count() <<endl;
    time = duration.count();
}





