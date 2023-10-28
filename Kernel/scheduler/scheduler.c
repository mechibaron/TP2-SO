#include "scheduler.h" // Incluye el archivo de encabezado de tu scheduler

//FIX: cambier mallocs por nuestro memory manager.

#define READY 0
#define BLOCKED 1
#define RUNNING 2

#define NUMBER_OF_PRIORITIES 9
#define DEFAULT_PRIORITY 1
int priorities[NUMBER_OF_PRIORITIES] = {9, 8, 7, 6, 5, 4, 3, 2, 1};

// Schelduler states
int processAmount = -1;
unsigned int processReadyCount = 0;
// pid_t dummyProcessPid = NULL;
char proccessBeingRun = 0;
int readyProcessAmount = 0;

Node* active = NULL;
Node* expired = NULL;

void addToActiveProcesses(Node* newProcess) {
    if (active == NULL) {
        // Si la lista de procesos activos está vacía, el nuevo proceso se convierte en el primero.
        newProcess->next = NULL;
        active = newProcess;
    } else {
        // Si hay procesos activos en la lista, se agrega el nuevo proceso al principio.
        newProcess->next = active;
        active = newProcess;
    }    char* argv[] = {"Initial Task"};
}

void addToExpiredProcesses(Node *newProcess) {
    newProcess->next = NULL;
    if (expired == NULL) {
        // Si la lista de procesos expirados está vacía, el nuevo proceso se convierte en la lista.
        expired = newProcess;
    } else {
        Node *current = expired;
        Node *previous = NULL;

        // Encuentra la ubicación correcta en la lista para insertar el nuevo proceso según p dsu prioridad.
        while (current != NULL && newProcess->process.priority <= current->process.priority) {
            previous = current;
            current = current->next;
        }

    // Verificar si el proceso debe agregarse a la lista de procesos activos o expirados
    if (newProcess->process.priority >= DEFAULT_PRIORITY) {
        // Agregar el proceso a la lista de procesos activos
        addToActiveProcesses(newProcess);
        processReadyCount++;
    } else {
        // Agregar el proceso a la lista de procesos expirados
        addToExpiredProcesses(newProcess);
    }
        if (previous == NULL) {
            // El nuevo proceso tiene la prioridad más alta, por lo que se convierte en el nuevo inicio de la lista.
            newProcess->next = expired;
            expired = newProcess;
        } else {
            // Inserta el nuevo proceso en la posición adecuada en la lista.
            previous->next = newProcess;
            newProcess->next = current;
        }
    }
}

// Función para copiar un arreglo de cadenas de caracteres
char** copy_argv(int argc, char** argv) {
    if (argc <= 0 || argv == NULL) {
        return NULL; // Manejar casos de error
    }

    char** new_argv = (char**)malloc(argc * sizeof(char*));
    if (new_argv == NULL) {
        perror("Error al asignar memoria para los argumentos");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < argc; i++) {
        new_argv[i] = (char*)malloc(strlen(argv[i]) + 1); // +1 para el carácter nulo
        if (new_argv[i] == NULL) {
            perror("Error al asignar memoria para los argumentos");
            exit(EXIT_FAILURE);
        }
        strcpy(new_argv[i], argv[i]);
    }

    return new_argv;
}



void contextSwitch(Node* currentProcess, Node* nextProcess) {
    if (currentProcess != NULL) {
        // Guarda el contexto del proceso actual y cambia su estado
        currentProcess->process.status = READY;
        // Aquí deberías guardar el contexto actual (puntero de pila, registros, etc.)

        // Si es necesario, actualiza el tiempo de CPU utilizado por el proceso actual
        // y realiza otras operaciones de mantenimiento

        // Guardar el contexto del proceso actual y cambiar su estado a READY
    }

    if (nextProcess != NULL) {
        // Restaurar el contexto del próximo proceso y cambiar su estado
        nextProcess->process.status = RUNNING;
        // Aquí deberías restaurar el contexto (puntero de pila, registros, etc.)

        // Actualizar el tiempo de CPU utilizado por el próximo proceso
    }
}



Node* selectNextProcess() {
    Node* current = active;
    Node* selected = NULL;

    while (current != NULL) {
        if (current->process.status == READY) {
            if (selected == NULL || current->process.priority > selected->process.priority) {
                selected = current;
            }
        }
        current = current->next;
    }

    return selected;
}


void schedule() {
    // Seleccionar el próximo proceso a ejecutar (puedes usar una política simple)
    Node* nextProcess = selectNextProcess();

    if (nextProcess != NULL) {
        // Realizar el cambio de contexto
        contextSwitch(active, nextProcess);
        active = nextProcess;
    }
}




PCB *getProcess(pid_t pid) {
    Node *current = active;

    // Busca el proceso en la lista de procesos activos
    while (current != NULL && current->process.pid != pid) {
        current = current->next;
    }

    if (current != NULL) {
        // Si se encuentra el proceso en la lista de procesos activos, devuelve su PCB
        return &(current->process);
    }

    current = expired;

    // Busca el proceso en la lista de procesos expirados
    while (current != NULL && current->process.pid != pid) {
        current = current->next;
    }

    if (current != NULL) {
        // Si se encuentra el proceso en la lista de procesos expirados, devuelve su PCB
        return &(current->process);
    }

    // Si el proceso no se encuentra en ninguna lista, retorna NULL para indicar que no existe
    return NULL;
}

// Función para crear un nuevo proceso
pid_t createProcess(uint64_t rip, int argc, char** argv) {
    // Asignar memoria para la estructura PCB y otros recursos del proceso
    Node* newProcess = (Node*)malloc(sizeof(PCB));

    if (newProcess == NULL) {
        perror("Error al asignar memoria para el proceso");
        return -1; // Indica un error
    }
    
    // Configurar los campos del PCB con los valores necesarios
    newProcess->process.pid = processAmount++; // Debes tener una función para generar PIDs únicos
    newProcess->process.rip = rip; // Puntero de instrucción de inicio del proceso
    newProcess->process.argc = argc;
    newProcess->process.argv = copy_argv(argc, argv);

    // Otras inicializaciones
    newProcess->process.status = READY; // Puede ser READY, BLOCKED, o cualquier otro estado inicial
    // newProcess->quantumsLeft = INITIAL_QUANTUMS; // Definir el número inicial de quantums
    // newProcess->stackBase = allocateStackMemory(); // Debes tener una función para asignar memoria de pila
    newProcess->process.priority = DEFAULT_PRIORITY; // Asignar una prioridad por defecto
    
    
    // Verificar si el proceso debe agregarse a la lista de procesos activos o expirados
    if (newProcess->process.priority >= DEFAULT_PRIORITY) {
        // Agregar el proceso a la lista de procesos activos
        addToActiveProcesses(newProcess);
        processReadyCount++;
    } else {
        // Agregar el proceso a la lista de procesos expirados
        addToExpiredProcesses(newProcess);
    }


    return newProcess->process.pid; // Devuelve el PID del nuevo proceso
}

uint64_t getCurrentPid()
{
    if (active != NULL)
    {
        return active->process.pid;
    }
    return -1;
}

int killProcess(pid_t pid) {
    PCB *process = getProcess(pid);
    if (process != NULL) {
        // Realiza operaciones de terminación y limpieza utilizando el puntero 'process'.
        // ...
        return 0;
    }
    return -1; // Proceso no encontrado.
}

int changePriority(pid_t pid, int newPriority) {
    if (newPriority < 0 || newPriority > 9)
    {
        return -1;
    }
    PCB *process = getProcess(pid);
    if (process != NULL) {
        process->priority = newPriority;

        return 0;
    }
    return -1; // Proceso no encontrado.
}


int blockProcess(pid_t pid) {
    PCB *process = getProcess(pid);
    if (process != NULL) {
        if (process->status == READY) {
            process->status = BLOCKED;
            processReadyCount--;
            return 0;  // Éxito
        }
    }
    return -1;  // Error: no se pudo bloquear el proceso
}

int unblockProcess(pid_t pid) {
    PCB *process = getProcess(pid);
    if (process != NULL) {
        if (process->status == BLOCKED) {
            process->status = READY;
            processReadyCount++;
            return 0;  // Éxito
        }
    }
    return -1;  // Error: no se pudo desbloquear el proceso
}

int main(int argc, char *argv[])
{

    return 0;
}
