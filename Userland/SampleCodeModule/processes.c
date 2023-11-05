// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <semaphore.h>
// #include <string.h>
// #include <signal.h>
#include "include/syslib.h"

#define MAX_PHILOSOPHERS 10
#define EAT_SECONDS 1
#define THINKING 0
#define EATING 1
#define HUNGRY 2

typedef struct {
    int state;
    int pid;
} Philosopher;

Philosopher philosophers[MAX_PHILOSOPHERS];
int philosopher_count = 0;
sem_t mutex;
sem_t philoSem[MAX_PHILOSOPHERS];

void check_availability(int index) {
    if (philosophers[index].state == HUNGRY &&
        philosophers[(index + philosopher_count - 1) % philosopher_count].state != EATING &&
        philosophers[(index + 1) % philosopher_count].state != EATING) {
        philosophers[index].state = EATING;
        printf("Philosopher %d is EATING\n", index);
        sem_post(&philoSem[index]);
    }
}

void take_chopsticks(int index) {
    sem_wait(&mutex);
    philosophers[index].state = HUNGRY;
    check_availability(index);
    sem_post(&mutex);
    sem_wait(&philoSem[index]);
}

void put_chopsticks(int index) {
    sem_wait(&mutex);
    philosophers[index].state = THINKING;
    check_availability((index + philosopher_count - 1) % philosopher_count);
    check_availability((index + 1) % philosopher_count);
    sem_post(&mutex);
}

void add_philosopher() {
    if (philosopher_count < MAX_PHILOSOPHERS) {
        sem_init(&philoSem[philosopher_count], 0, 0);
        philosophers[philosopher_count].state = THINKING;

        char seat[3];
        snprintf(seat, sizeof(seat), "%d", philosopher_count);
        char* argv[] = {"philosopher", seat, NULL};

        int pid = fork();
        if (pid == 0) {
            // Child process (philosopher)
            execv("./philosopher", argv);
        } else {
            // Parent process
            philosophers[philosopher_count].pid = pid;
            philosopher_count++;
        }
    } else {
        printf("Error: Cannot add more philosophers. Maximum reached.\n");
    }
}

void remove_philosopher() {
    if (philosopher_count > 0) {
        philosopher_count--;
        sem_close(&philoSem[philosopher_count]);

        if (philosophers[philosopher_count].state == EATING || philosophers[philosopher_count].state == THINKING) {
            put_chopsticks((philosopher_count + philosopher_count - 1) % philosopher_count);
            put_chopsticks((philosopher_count + 1) % philosopher_count);
        }

        sys_kill(philosophers[philosopher_count].pid);
    } else {
        printf("Error: Cannot remove more philosophers. No philosophers left.\n");
    }
}

void terminate() {
    while (philosopher_count > 0) {
        remove_philosopher();
    }
}

void print_state() {
    printf("Philosopher Table: ");
    for (int i = 0; i < philosopher_count; i++) {
        printf("Philosopher %d: %c ", i, philosophers[i].state == EATING ? 'E' : '.');
    }
    printf("\n");
}

int main() {
    sem_init(&mutex, 0, 1);

    printf("Press 'a' to add a philosopher.\nPress 'r' to remove a philosopher.\nPress 'q' to exit.\n");

    char c;
    while (1) {
        c = getchar();
        if (c == 'a') {
            add_philosopher();
            print_state();
        } else if (c == 'r') {
            remove_philosopher();
            print_state();
        } else if (c == 'q') {
            terminate();
            break;
        }
    }

    sem_destroy(&mutex);
    return 0;
}
