#include <stdio.h>
#include <string.h>

void print_file(const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open file %s\n", filename);
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
}

// driver code
int main(int argc, char* argv[])
{
    FILE* file;
    char ch;

    // Check if filename was given or not
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    // calling function to read file
    print_file(argv[1]);

    return 0;
}
