#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

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
        close(fileDescriptor[0]);
        int randomNumber = rand() % 8 + 5;
        Renna r;
        for (int i = 0; i < randomNumber; i++) {
            strcpy(r.nome, "Renna_");
            r.nome[6] = (i + 1) + '0';
            r.velocità = rand() % 51 + 50;
            r.resistenza = rand() % 10 + 1;
            write(fileDescriptor[1], &r, sizeof(Renna));
            sleep(1);
        }
        strcpy(r.nome, "Renna_Sentinella");
        r.velocità = -1;
        r.resistenza = rand() % 10 + 1;
        write(fileDescriptor[1], &r, sizeof(Renna));
        close(fileDescriptor[1]);
        exit(EXIT_SUCCESS);
    } else {
        close(fileDescriptor[1]);
        wait(NULL);
    }
    return 0;
}