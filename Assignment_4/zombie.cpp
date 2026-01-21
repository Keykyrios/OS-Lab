#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed." << endl;
        return 1;
    }
    else if (pid == 0) {
        cout << "Child process (PID: " << getpid() << ") is exiting..." << endl;
        exit(0); 
    }
    else {
        cout << "Parent process (PID: " << getpid() << ") created child (PID: " << pid << ")." << endl;
        cout << "Parent is sleeping for 15 seconds. Check 'ps -l' now to see the Zombie (Z)." << endl;
       
        sleep(15);
       
        wait(NULL);
        cout << "Parent woke up and cleaned up the child process." << endl;
    }

    return 0;
}
