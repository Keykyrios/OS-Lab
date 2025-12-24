#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ./wc <filename>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        perror("Error opening file");
        return 1;
    }

    long long lines = 0, words = 0, characters = 0;
    char ch;
    bool inWord = false;


    while (file.get(ch)) {
        characters++;


        if (ch == '\n') {
            lines++;
        }


        if (isspace(ch)) {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            words++;
        }
    }


    if (characters > 0 && lines == 0) lines = 1; 

    cout << "Lines: " << lines << endl;
    cout << "Words: " << words << endl;
    cout << "Characters: " << characters << endl;

    file.close();
    return 0;
}
