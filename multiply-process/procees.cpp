/*
	Crear "n" procesos con fork() en C++
	por Daniel Gacitúa
	<danielgacituav@gmail.com>
*/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

int main (int argc, char** argv) {
	static const int PROC = 5;		// PROC fija la cantidad de procesos a crear

	int status, procNum;			// procNum almacena el número del proceso
	pid_t pid;

	for (procNum=0; procNum<PROC; procNum++) {
		pid = fork();				// se hace fork()
		
		if (pid==0) {				// si el proceso se crea bien, terminamos el ciclo for
			break;
		}
		else if (pid==-1) {			// si hay error, se aborta la operación
			perror("ERROR al hacer fork()");
			exit(1);
			break;
		}
	}

	if (pid==0) {			// Lógica del Hijo
		cout << "soy el proceso " << procNum << " " << getpid() << endl;
		exit(0);
	}
	else {					// Lógica del Padre
		for (int i=0; i<PROC; i++) {		// esperamos a que todos los hijos terminen (código mejorado)
    		if ((wpid = wait(NULL)) >= 0) {
    			cout << "Proceso " << wpid << " terminado" << endl;
    		}
    	}

		cout << "Soy el padre " << getpid() << endl;
	}

	return 0;		// Fin del programa
}