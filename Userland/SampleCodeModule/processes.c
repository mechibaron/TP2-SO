#include "include/syslib.h"

#define MAX_PHYLOSOPHERS 20
#define EAT_SECONDS 1

#define THINKING 0
#define EATING 1
#define HUNGRY 2

sem_t mutexSem;
sem_t phyloSem[MAX_PHYLOSOPHERS];

int phyloQty = 0;

int state[MAX_PHYLOSOPHERS];
pid_t phyloPids[MAX_PHYLOSOPHERS];
char phyloNames[MAX_PHYLOSOPHERS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};

void takeChopsticks(int index);
void putChopsticks(int index);
void checkAvailability(int index);
int left(int index);
int right(int index);
void printState();

void phylo(uint64_t argc, char *argv[]) {
    fprintf(STDOUT, "Welcome to the dining philosophers' problem!\n");

    mutexSem = sem_open(30, 1);

    for (int i = 0; i < MAX_PHYLOSOPHERS; i++) {
        phyloSem[i] = 0;
    }

    fprintf(STDOUT, "Press 'a' to add a philosopher, 'r' to remove a philosopher, or 'q' to exit.\n");
    char c;
    while ((c = getChar()) != 0) {
        switch (c) {
            case 'a':
                if (phyloQty < MAX_PHYLOSOPHERS) {
                    addPhylo(phyloQty);
                } else {
                    fprintf(STDOUT, "Error: Cannot add more philosophers.\n");
                }
                break;
            case 'r':
                if (phyloQty > 1) {
                    deletePhylo(phyloQty - 1);
                } else {
                    fprintf(STDOUT, "Error: Cannot remove more philosophers.\n");
                }
                break;
            case 'q':
                finishEating();
                sem_close(mutexSem);
                sys_exit(0);
                break;
        }
    }

    fprintf(STDOUT, "Goodbye!\n");
    finishEating();
    sem_close(mutexSem);
    sys_exit(0);
}

int addPhylo(int index) {
    if (index >= MAX_PHYLOSOPHERS) {
        return -1;
    }

    sem_wait(mutexSem);

    state[index] = THINKING;
    char seatIdx[3];
    uintToBase(index, seatIdx, 10);
    char *argv[] = {"phylo", seatIdx, "&"};

    phyloSem[index] = sem_open(index + 100, 1);
    if (phyloSem[index] == 0) {
        return -1;
    }

    phyloPids[index] = sys_exec((uint64_t)&phyloProcess, 2, argv);
    phyloQty++;

    sem_post(mutexSem);
    return 0;
}

void phyloProcess(int argc, char *argv[]) {
    int seat = atoi((char *)argv[1]);

    while (1) {
        sleep(EAT_SECONDS);
        takeChopsticks(seat);
        sleep(EAT_SECONDS);
        putChopsticks(seat);
    }
}

void takeChopsticks(int index) {
    sem_wait(mutexSem);

    state[index] = HUNGRY;
    checkAvailability(index);

    sem_post(phyloSem[index]);
    sem_wait(mutexSem);
}

void putChopsticks(int index) {
    sem_wait(mutexSem);

    state[index] = THINKING;
    checkAvailability(left(index));
    checkAvailability(right(index));

    sem_post(mutexSem);
}

void checkAvailability(int index) {
    if (state[index] == HUNGRY && state[left(index)] != EATING && state[right(index)] != EATING) {
        state[index] = EATING;
        printState();
        sem_post(phyloSem[index]);
    }
}

int left(int index) {
    return (index == 0) ? phyloQty - 1 : index - 1;
}

int right(int index) {
    return (index == phyloQty - 1) ? 0 : index + 1;
}

void deletePhylo(int index) {
    if (index < 0 || phyloQty == 0) {
        return;
    }

    sem_wait(mutexSem);

    int eating = (state[index] == EATING);

    if (sem_close(phyloSem[index]) == -1) {
        fprintf(STDOUT, "Error closing philosopher's semaphore\n");
    }

    sys_kill(phyloPids[index]);
    phyloQty--;

    if (eating) {
        checkAvailability(index - 1);
        checkAvailability(0);
    }

    sem_post(mutexSem);
}

void finishEating() {
    while (phyloQty > 0) {
        if (sem_close(phyloSem[phyloQty - 1]) == -1) {
            fprintf(STDOUT, "Error closing philosopher's semaphore\n");
        }

        sys_kill(phyloPids[phyloQty - 1]);
        phyloQty--;
    }
}

void printState() {
    char output[2];
    for (int i = 0; i < phyloQty; i++) {
        state[i] == EATING ? (output[0] = 'E') : (output[0] = '.');
        output[1] = '\0';
        fprintf(STDOUT, output);
    }
    fprintf(STDOUT, "\n");
}
