#include "include/processes.h"
#include "include/libc.h"
#include "include/types.h"

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

void wc(int argc, char *argv[])
{
    int c;
    int lines = 0;
    while ((c = getChar()) != EOF)
    {
        if (c == '\n')
        {
            lines++;
        }
    }
    fprintf(STDOUT, "%d lines detected\n", lines);
}

void getTime(int argc, char *argv[])
{
    sysTime_t time;
    char buffer[64];
    sys_time(&time);

    putChar('\n');
    uintToBase(time.hours, buffer, 10);
    puts(buffer);
    putChar(':');
    uintToBase(time.minutes, buffer, 10);
    puts(buffer);
    putChar(':');
    uintToBase(time.seconds, buffer, 10);
    puts(buffer);
    putChar('\n');

    uintToBase(time.day, buffer, 10);
    puts(buffer);
    putChar('/');
    uintToBase(time.month, buffer, 10);
    puts(buffer);
    putChar('/');
    uintToBase(time.year + 2000, buffer, 10);
    puts(buffer);
}

void filter(int argc, char *argv[])
{

    char vowels[] = {'a', 'e', 'i', 'o', 'u'};
    char capitalVowels[] = {'A', 'E', 'I', 'O', 'U'};
    int c;
    while ((c = getChar()) != EOF)
    {
        for (int k = 0; k < 5; k++)
        {
            if (c == vowels[k])
            {
                fprintf(STDOUT, "%c", vowels[k]);
            }
            if (c == capitalVowels[k])
            {
                fprintf(STDOUT, "%c", capitalVowels[k]);
            }
        }
    }
}

void cat(int argc, char *argv[])
{
    int i = 0, c = getChar();
    while (c != EOF)
    {
        if (c == '\b' && i > 0)
        {
            i--;
            putChar(c);
        }
        else if (c == '\b' && i == 0)
        {
            // no hago nada
        }
        else
        {
            i++;
            putChar(c);
        }
        c = getChar();
    }
}
void getProcessesInfo(int argc, char *argv[])
{
    processInfo *current = sys_ps();
    char *toPrint;

    while (current != NULL)
    {
        fprintf(STDOUT, "PID: %d\n", current->pid);
        fprintf(STDOUT, "Priority: %d\n", current->priority);
        fprintf(STDOUT, "Stack Base: 0x%x\n", current->stackBase);
        fprintf(STDOUT, "Status: %s\n\n", (current->status) ? "BLOCKED" : "READY");
        sys_memFree(current);
        current = current->next;
    }
}
void help(int argc, char *argv[])
{
    const char *helpstring =
        "cat                  Replicates whatever you input to the shell.\n"
        "phylo                Simulates famous synchronization phylosophers problem.\n"
        "ps                   Prints information about current process.\n"
        "loop                 Prints a greeting every certain period of time.\n"
        "filter               Filters input to print only its vowels.\n"
        "block <pid>          Blocks/unblocks process with pid = <pid>.\n"
        "kill <pid>           Kills process with pid = <pid>.\n"
        "nice <pid> <prio>    Changes priority of process with pid = <pid> to <prio>.\n"
        "wc                   Prints the number of newlines from input.\n"
        "help                 Provides help information for commands.\n"
        "time                 Command to display the system day and time.\n"
        "COMMAND1|COMMAND2    The \"|\" operand allows the output of the first command\n"
        "                     to be the input of the second command. CTRL+D sends an EOF.\n"
        "test_mm <max-mem>    Tests memory manager with <max-mem> bytes.\n"
        "test_priority        Tests priority changes of processes.\n"
        "test_processes <max-proc>    Tests process creation with <max-proc>.\n"
        "test_sync <max-proc> <sem-flag>    Tests synchronization of processes.\n";

    puts(helpstring);
}

void inforeg(int argc, char *argv[])
{
    static char *registers[18] = {"R15", "R14", "R13", "R12", "R11", "R10", "R9 ", "R8 ", "RSI", "RDI", "RBP", "RDX", "RCX", "RBX", "RAX", "RIP", "RFL", "RSP"};

    uint64_t regval[18];
    int sysret = sys_getregs(regval);
    if (sysret == 0)
    {
        puts("No registers to print.\n");
        return;
    }
    char buffer[64];
    for (int i = 0; i < 18; i++)
    {
        puts(registers[i]);
        puts(": 0x");
        uintToBase(regval[i], buffer, 16);
        puts(buffer);
        putChar('\n');
    }
}


void loopProcess(int argc, char *argv[])
{
    int secs = 1;
    pid_t currentPid = sys_getCurrentPid();
    while (1)
    {
        wait(secs);
        fprintf(STDOUT, "McWhiggin manda saludos al proceso identificable por el siguiente PID: %d\n", (int)currentPid);
    }
}

void killProcess(int argc, char *argv[])
{
    if (argc != 2)
    {
        puts("kill should only receive 1 argument");
        return;
    }
    if (sys_kill(atoi(argv[1])) == 0)
    {
        fprintf(STDOUT, "Killed PID: %d\n", atoi(argv[1]));
    }
    else
    {
        fprintf(STDOUT, "Failed to kill PID: %d\n", atoi(argv[1]));
    }
}

void niceProcess(int argc, char *argv[])
{
    if (argc != 3)
    {
        puts("nice should only receive 2 argument");
        return;
    }
    int a1 = atoi(argv[1]);
    int a2 = atoi(argv[2]);
    if (sys_nice((pid_t)a1, a2))
    {
        fprintf(STDOUT, "Changed priority of PID: %d , to %d\n", a1, a2);
    }
    else
    {
        puts("Failed to chang priority\n");
    }
}


void blockProcess(int argc, char *argv[])
{
    if (argc != 2)
    {
        puts("block should only receive 1 arguments");
        return;
    }
    int a = atoi(argv[1]);
    int retValue = sys_changeProcessStatus(a);

    if (retValue == READY)
    {
        fprintf(STDOUT, "PID: %d unblocked\n", a);
    }
    else if (retValue == BLOCKED)
    {
        fprintf(STDOUT, "PID: %d blocked\n", a);
    }
    else
    {
        puts("Failed to block/unblock the process\n");
    }
}
