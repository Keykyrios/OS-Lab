#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ./collatz <positive_integer>" << endl;
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        cout << "Please enter a positive integer." << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed." << endl;
        return 1;
    }
    else if (pid == 0) {
        cout << "Child Process: " << n;
        while (n != 1) {
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = 3 * n + 1;
            }
            cout << ", " << n;
        }
        cout << endl;
    }
    else {
        wait(NULL);
        cout << "Parent Process: Child completed." << endl;
    }

    return 0;
}
