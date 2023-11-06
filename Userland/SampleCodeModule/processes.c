#include <libc.h>
#include <processes.h>

#define MAX_PHILOSOPHERS 20
#define EAT_SECONDS 1

#define THINKING 0
#define EATING 1
#define HUNGRY 2

sem_t mutexSem;
sem_t chopstickSem[MAX_PHILOSOPHERS];

int philosopherStates[MAX_PHILOSOPHERS];
pid_t philosopherPids[MAX_PHILOSOPHERS];

char philosopherNames[MAX_PHILOSOPHERS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
int philosopherCount = 0;

void takeChopsticks(int philosopher);
void putChopsticks(int philosopher);
void checkAvailability(int philosopher);
int left(int philosopher);
int right(int philosopher);
void printState();

void philosopherProcess(int argc, char *argv[]) {
    int philosopher = atoi(argv[1]);

    while (1) {
        sleep(EAT_SECONDS);
        takeChopsticks(philosopher);
        sleep(EAT_SECONDS);
        putChopsticks(philosopher);
    }
}

void addPhilosopher() {
    if (philosopherCount >= MAX_PHILOSOPHERS) {
        fprintf(STDOUT, "Cannot add more philosophers.\n");
        return;
    }

    sem_wait(mutexSem);

    philosopherStates[philosopherCount] = THINKING;

    char philosopherName[2];
    uintToBase(philosopherCount, philosopherName, 10);

    chopstickSem[philosopherCount] = sem_open(philosopherName, 1);
    if (chopstickSem[philosopherCount] == 0) {
        fprintf(STDOUT, "Failed to add philosopher.\n");
        return;
    }

    char *argv[] = {philosopherName};
    philosopherPids[philosopherCount] = sys_exec((uint64_t)philosopherProcess, 1, argv);

    fprintf(STDOUT, "Added philosopher %c\n", philosopherNames[philosopherCount]);

    philosopherCount++;
    sem_post(mutexSem);
}

int left(int philosopher) {
    // La función 'left' devuelve el índice del palillo a la izquierda del filósofo 'philosopher'.
    return philosopher;
}

int right(int philosopher) {
    // La función 'right' devuelve el índice del palillo a la derecha del filósofo 'philosopher'.
    return (philosopher + 1) % philosopherCount;
}

void checkAvailability(int philosopher) {
    if (philosopherStates[philosopher] == HUNGRY &&
        philosopherStates[left(philosopher)] != EATING &&
        philosopherStates[right(philosopher)] != EATING) {
        // Si el filósofo está hambriento y sus vecinos no están comiendo,
        // el filósofo puede comenzar a comer.
        philosopherStates[philosopher] = EATING;
        sem_post(chopstickSem[philosopher]);
    }
}


void removePhilosopher() {
    if (philosopherCount == 0) {
        fprintf(STDOUT, "Failed, the number of philosophers must be greater than 0.\n");
        return;
    }

    sem_wait(mutexSem);

    philosopherCount--;

    sem_close(chopstickSem[philosopherCount]);
    sys_kill(philosopherPids[philosopherCount]);

    fprintf(STDOUT, "Removed philosopher %c\n", philosopherNames[philosopherCount]);

    sem_post(mutexSem);
}

void finishEating() {
    while (philosopherCount > 0) {
        sem_close(chopstickSem[philosopherCount - 1]);
        sys_kill(philosopherPids[philosopherCount - 1]);
        philosopherCount--;
    }
}

void printState() {
    for (int i = 0; i < philosopherCount; i++) {
        philosopherStates[i] == EATING ? fprintf(STDOUT, "E ") : fprintf(STDOUT, ". ");
    }
    fprintf(STDOUT, "\n");
}

void takeChopsticks(int philosopher) {
    sem_wait(mutexSem); // Acceder a la región crítica

    philosopherStates[philosopher] = HUNGRY;
    checkAvailability(philosopher); // Verificar si puede tomar los palillos

    sem_post(mutexSem); // Salir de la región crítica
}

void putChopsticks(int philosopher) {
    sem_wait(mutexSem); // Acceder a la región crítica

    philosopherStates[philosopher] = THINKING;
    checkAvailability(left(philosopher)); // Verificar si el filósofo de la izquierda puede tomar el palillo derecho
    checkAvailability(right(philosopher)); // Verificar si el filósofo de la derecha puede tomar el palillo izquierdo

    sem_post(mutexSem); // Salir de la región crítica
}


void phylo(uint64_t argc, char *argv[]) {
    fprintf(STDOUT, "Welcome to the dining philosophers' problem!\n");

    mutexSem = sem_open("philosopher_mutex", 1);

    for (int i = 0; i < MAX_PHILOSOPHERS; i++) {
        chopstickSem[i] = 0;
    }

    fprintf(STDOUT, "Press 'a' to add a philosopher, 'r' to remove a philosopher, or 'q' to exit.\n");

    char c;
    while ((c = getChar()) != EOF) {
        if (c == 'a' || c == 'A') {
            addPhilosopher();
        } else if (c == 'r' || c == 'R') {
            removePhilosopher();
        } else if (c == 'q' || c == 'Q') {
            break;
        }
    }

    finishEating();

    for (int i = 0; i < MAX_PHILOSOPHERS; i++) {
        if (chopstickSem[i] != 0) {
            sem_close(chopstickSem[i]);
        }
    }

    sem_close(mutexSem);

    sys_exit(0);
}
