#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

/* --
 * Do nsweeps sweeps of Jacobi iteration on a 1D Poisson problem
 * 
 *    -u'' = f
 *
 * discretized by n+1 equally spaced mesh points on [0,1].
 * u is subject to Dirichlet boundary conditions specified in
 * the u[0] and u[n] entries of the initial vector.
 */

using namespace std;

//Jacobi with threads optimization

void jacobithread(int nsweeps, int n, double* u, double* f,double* utmp)
{

    int i, sweep;
    double h  = 1.0 / n;
    double h2 = h*h;
    //double *utmp = (double*) malloc( (n+1) * sizeof(double) );
    ThreadPool pool(nsweeps);
    auto start = std::chrono::high_resolution_clock::now();

    /* Fill boundary conditions into utmp */
    utmp[0] = u[0];
    utmp[n] = u[n];

    for (sweep = 0; sweep < nsweeps; sweep += 2) {
        /* Old data in u; new data in utmp */
        vector<future<int>> results;
        results.emplace_back(pool.enqueue([&utmp,&u,&h2,&f,&i,&n] {
        for (i = 1; i < n; ++i)
            utmp[i] = (u[i-1] + u[i+1] + h2*f[i])/2;
        
        /* Old data in utmp; new data in u */
        for (i = 1; i < n; ++i)
            u[i] = (utmp[i-1] + utmp[i+1] + h2*f[i])/2;
        return 1;
        })
        );
    }

    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << duration.count() <<endl;
    
}


//jacobi in a serial implementation
void jacobiseri(int nsweeps, int n, double* u, double* f,double* utmp)
{

    int i, sweep;
    double h  = 1.0 / n;
    double h2 = h*h;
    auto start = std::chrono::high_resolution_clock::now();

    /* Fill boundary conditions into utmp */
    utmp[0] = u[0];
    utmp[n] = u[n];

    for (sweep = 0; sweep < nsweeps; sweep += 2) {
        /* Old data in u; new data in utmp */
        for (i = 1; i < n; ++i)
            utmp[i] = (u[i-1] + u[i+1] + h2*f[i])/2;
        
        /* Old data in utmp; new data in u */
        for (i = 1; i < n; ++i)
            u[i] = (utmp[i-1] + utmp[i+1] + h2*f[i])/2;

    }

    free(utmp);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << duration.count()<<endl;
    
}




int main(int argc, char** argv)
{
    int i;
    int n, nsteps;
    double* u;
    double* f;
    double h;
    double* utmp = (double*) malloc( (10) * sizeof(double) );

    /* Process arguments */
    n      = stoi(argv[1]);
    nsteps = 100;
    h      = 1.0/n;

    /* Allocate and initialize arrays */
    u = (double*) malloc( (n+1) * sizeof(double) );
    f = (double*) malloc( (n+1) * sizeof(double) );
    memset(u, 0, (n+1) * sizeof(double));
    for (i = 0; i <= n; ++i)
        f[i] = i * h;

    /* Run the solver */
    jacobithread(nsteps, n, u, f,utmp);

    /*run in a serail */
    jacobiseri(nsteps, n, u, f,utmp);

    free(f);
    free(u);
    return 0;
}
