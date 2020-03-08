#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "err.h"

// Set DEPTH to desired value

#define DEPTH 4


using namespace std;

int main ()
{
  pid_t pid;
  int i;

  cout << "My process id = " << getpid() << endl;

  for (i=1 ; i <= DEPTH ; i++) {

    pid = fork();    // Fork

    if ( pid ) {
       break;        // Don't give the parent a chance to fork again
    }
    cout << "Child #" << getpid() << endl; // Child can keep going and fork once
  }

  wait(NULL);        // Don't let a parent ending first end the tree below
  return 0;
}