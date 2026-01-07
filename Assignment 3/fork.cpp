#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int value = 5; 

int main() {
    pid_t pid = fork(); 

    if (pid == 0) { 
        value += 15; 
        return 0;
    } else if (pid > 0) { 
        wait(NULL); 
        std::cout << "PARENT: value = " << value << std::endl; 
    return 0;
}
}
