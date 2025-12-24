#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int main() {
    DIR *dir = opendir("/proc");
    struct dirent *entry;

    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) > 0) {
            char path[1024];
            snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);

            FILE *f = fopen(path, "r");
            if (f == NULL) continue;

            char comm[256];
            long pid;
            if (fscanf(f, "%ld %s", &pid, comm) == 2) {
                printf("%ld %s\n", pid, comm);
            }

            fclose(f);
        }
    }

    closedir(dir);
    return 0;
}

