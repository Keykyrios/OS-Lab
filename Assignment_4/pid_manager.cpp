#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#define MIN_PID 300
#define MAX_PID 5000


int pid_map[MAX_PID - MIN_PID + 1];

int allocate_map(void) {
    memset(pid_map, 0, sizeof(pid_map));
    return 1; // Successful
}

int allocate_pid(void) {
    for (int i = 0; i <= MAX_PID - MIN_PID; i++) {
        if (pid_map[i] == 0) {
            pid_map[i] = 1; // Mark as in use
            return i + MIN_PID;
        }
    }
    return -1; // No PID available
}

void release_pid(int pid) {
    if (pid < MIN_PID || pid > MAX_PID) {
        cout << "Invalid PID: " << pid << endl;
        return;
    }
    int index = pid - MIN_PID;
    pid_map[index] = 0; // Mark as available
    cout << "Released PID: " << pid << endl;
}

int main() {
    if (allocate_map() == 1) {
        cout << "PID Map Initialized." << endl;
    }

    int pid1 = allocate_pid();
    cout << "Allocated PID: " << pid1 << endl;
   
    int pid2 = allocate_pid();
    cout << "Allocated PID: " << pid2 << endl;

    int pid3 = allocate_pid();
    cout << "Allocated PID: " << pid3 << endl;

    release_pid(pid2);

    int pid4 = allocate_pid();
    cout << "Allocated PID (should be old pid2): " << pid4 << endl;

    return 0;
}
