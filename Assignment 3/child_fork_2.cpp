#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int SIZE = 5;
int nums[SIZE] = {0, 1, 2, 3, 4};

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        // Error occurred
        std::cerr << "Fork Failed" << std::endl;
        return 1;
    }
    else if (pid == 0) { // Child process
        for (int i = 0; i < SIZE; i++) {
            nums[i] *= -i;
            std::cout << "CHILD: " << nums[i] << std::endl; // LINE X
        }
    }
    else { // Parent process
        wait(NULL);
        for (int i = 0; i < SIZE; i++) {
            std::cout << "PARENT: " << nums[i] << std::endl; // LINE Y
        }
    }

    return 0;
}
