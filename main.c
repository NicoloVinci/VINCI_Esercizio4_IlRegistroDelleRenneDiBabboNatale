#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <signal.h>

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
        Renna renne[randomNumber + 1];
        Renna r;
        for (int i = 0; i < randomNumber; i++) {
            strcpy(r.nome, "Renna_");
            if (i == 9) {
                r.nome[6] = '1';
                r.nome[7] = '0';
                r.nome[8] = '\0';
            } else if (i == 10) {
                r.nome[6] = '1';
                r.nome[7] = '1';
                r.nome[8] = '\0';
            } else if (i == 11) {
                r.nome[6] = '1';
                r.nome[7] = '2';
                r.nome[8] = '\0';
            } else {
                r.nome[6] = (i + 1) + '0';
                r.nome[7] = '\0';
            }
            r.velocità = rand() % 51 + 50;
            r.resistenza = rand() % 10 + 1;
            renne[i] = r;
        }
        strcpy(r.nome, "Renna_Sentinella");
        r.velocità = -1;
        r.resistenza = rand() % 10 + 1;
        renne[randomNumber] = r;
        for (int i = 0; i <= randomNumber; i++) {
            if (write(fileDescriptor[1], &renne[i], sizeof(Renna)) != sizeof(Renna)) {
                perror("write to santa");
                exit(EXIT_FAILURE);
            }
        }
        close(fileDescriptor[1]);
        exit(EXIT_SUCCESS);
    } else {
        close(fileDescriptor[1]);
        Renna r;
        Renna *renne = malloc(sizeof(Renna));
        int i = 0;
        ssize_t n;
        while ((n = read(fileDescriptor[0], &r, sizeof(Renna))) > 0) {
            if (n != sizeof(Renna)) {
                perror("read from elf");
                exit(EXIT_FAILURE);
            }
            if (r.velocità == -1) {
                break;
            }
            renne = realloc(renne, sizeof(Renna) * (i + 1));
            renne[i] = r;
            i++;
        }
        if (n == -1) {
            perror("read from elf");
            exit(EXIT_FAILURE);
        }
        close(fileDescriptor[0]);
        wait(NULL);
    }
    return 0;
}