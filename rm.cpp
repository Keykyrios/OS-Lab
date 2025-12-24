#include <iostream>
#include <cstdio> 
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ./rm <filename>" << endl;
        return 1;
    }

    if (remove(argv[1]) == 0) {
        cout << "File deleted successfully" << endl;
    } else {
        perror("Error deleting file");
    }

    return 0;
}
