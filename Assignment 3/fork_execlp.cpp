#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork(); 

    if (pid == 0) { 
        execlp("/bin/ls", "ls", NULL);
        std::cout << "LINE J" << std::endl; 
    } else if (pid > 0) {
        wait(NULL); 
        std::cout << "Child Complete" << std::endl; 
    }
    return 0;
}
