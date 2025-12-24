#include <iostream>
#include <csignal> 
#include <cstdlib> 

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ./kill <PID>" << endl;
        return 1;
    }


    int pid = atoi(argv[1]);


    if (kill(pid, SIGKILL) == 0) {
        cout << "Process terminated successfully." << endl;
    } else {
        perror("Error terminating process");
    }

    return 0;
}
