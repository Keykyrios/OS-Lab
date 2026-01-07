#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid, pid1;

    /* fork a child process */
    pid = fork();

    if (pid < 0) { 
        /* error occurred */
        std::cerr << "Fork Failed" << std::endl;
        return 1;
    }
    else if (pid == 0) { 
        /* child process */
        pid1 = getpid();
        std::cout << "child: pid = " << pid << std::endl;   // LINE A
        std::cout << "child: pid1 = " << pid1 << std::endl; // LINE B
    }
    else { 
        /* parent process */
        pid1 = getpid();
        std::cout << "parent: pid = " << pid << std::endl;   // LINE C
        std::cout << "parent: pid1 = " << pid1 << std::endl; // LINE D
        wait(NULL);
    }

    return 0;
}
