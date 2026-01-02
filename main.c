#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

typedef struct {
    char nome[30];
    int velocità;  // km/h
    int resistenza; // 1-10
} Renna;

int main(void) {
    srand(time(NULL));
    int fileDescriptor[2];
    if (pipe(fileDescriptor) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t elf;
    if ((elf = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (elf == 0) {
        int randomNumber = rand() % 8 + 5;
        for (int i = 0; i < randomNumber; i++) {
            Renna r;
        }
    } else {
        close(fileDescriptor[1]);
        wait(NULL);
    }
    return 0;
}