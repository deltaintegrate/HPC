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
 
using namespace std;
 
void multmatseri(int **A, int **B, int **C, int N,int time);
void multmat(int **A, int **B, int **C, int N,int time);
 

int GetRandom(int max){
    srand(time(NULL));
    return rand() % max;
}

std::string GetTime(){
    auto nowtime = std::chrono::system_clock::now();
    std::time_t sleeptime = 
                std::chrono::system_clock::to_time_t(nowtime);
    return std::ctime(&sleeptime);
}


/*
void ExecuteThread(int id){
    
    auto nowtime = std::chrono::system_clock::now();
    std::time_t sleeptime = 
                std::chrono::system_clock::to_time_t(nowtime);
    tm myLocalTime = *localtime(&sleeptime);

    cout << "thread" << id << "Sleep time: " << std::ctime(&sleeptime) << "\n";
    cout << "Month" << myLocalTime.tm_mon << "\n";
    cout << "Day" << myLocalTime.tm_mday << "\n";
    cout << "Year" << myLocalTime.tm_year + 1900<< "\n";
    cout << "Hours" << myLocalTime.tm_hour << "\n";
    cout << "Minute" << myLocalTime.tm_min << "\n";
    cout << "second" << myLocalTime.tm_sec << "\n";

    std:: this_thread::sleep_for(std::chrono::seconds(GetRandom(3)));
    nowtime = std::chrono::system_clock::now(); 
    sleeptime = std::chrono::system_clock::to_time_t(nowtime);
    cout << "thread" << id << "Awake time: " << std::ctime(&sleeptime) << "\n";
}
*/

int main(int argcv, char * argv[]) {
 
    //ifstream label1 ("datos//datos.in"); // Abre el archivo de entrada de datos
    // Definición de variables y asignación dinámica de memoria
    unsigned int nthreads = std::thread::hardware_concurrency();
    srand (time(NULL));
    int i, j, k, N;
    int **A, **B, **C;
    int timeser,timethr;
;
    
    N = stoi(argv[1]);
    cout << "Empiezo....: " << N;
    A = new int* [N], B = new int* [N], C = new int* [N];

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
    /* USE OF THREAD  BY EXAMPLE
    thread th1(ExecuteThread,1);
    th1.join();
    thread th2(ExecuteThread,2);
    th2.join();
    */
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
    ThreadPool pool(thread::hardware_concurrency());
    auto start = std::chrono::high_resolution_clock::now();
    int j, k;
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                vector<future<int>> results;
                results.emplace_back(pool.enqueue([A,B,C,N,i,j] {
                C[i][j]=0;         
                for(int k=0; k<N; k++){

                    C[i][j]= C[i][j]+A[i][k]*B[k][j];
                }
                return 1;
                                                            })
                
                                    );

            }
        }
           
        
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "in Time of execute with thread "<< ": "<< duration.count() <<" microsegundos" <<endl <<endl;
    time = duration.count();
}

//serializacion

void multmatseri(int **A, int **B, int **C, int N,int time){
    auto start = std::chrono::high_resolution_clock::now();
    int i, j, k;
        for(i=0; i<N; i++){
            for(int j=0; j<N; j++){
                C[i][j]=0;         
                for(int k=0; k<N; k++){
                    C[i][j]= C[i][j]+A[i][k]*B[k][j];
                }
            }
        }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "in Time of execute without it "<< ": "<< duration.count() <<" microsegundos" <<endl <<endl;
    time = duration.count();
}







