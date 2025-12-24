#include <iostream>
#include <cstdio> // For remove()

using namespace std;

int main(int argc, char *argv[]) {
    // Check if filename is provided
    if (argc < 2) {
        cout << "Usage: ./rm <filename>" << endl;
        return 1;
    }

    // remove() deletes the file specified
    if (remove(argv[1]) == 0) {
        cout << "File deleted successfully" << endl;
    } else {
        perror("Error deleting file");
    }

    return 0;
}
