#include <n7OS/process.h>

#define MAX_PROCESSES 255 // Nomnbre maximum de processus

// Liste des processus = table des processus
process_t process_list[MAX_PROCESSES];

// Pid de l'execution en cours
pid_t current_pid = 0;

// Initialisation de la liste des processus
void init_process() {
    // TODO: PID 0 est le processus système, on en fait quoi ?

    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_list[i].state = LIBRE;
    }
}

// Fonction pour trouver un pid liste dans la liste des processus
pid_t find_free_pid() {
    for (pid_t i = 0; i < MAX_PROCESSES; i++) {
        if (process_list[i].state == LIBRE) {
            return i;
        }
    }
    return -1; // Aucun pid libre trouvé
}

pid_t getpid() {
    // On retourne le pid du processus en cours
    return current_pid;
}

void exit() {
    // On libère le processus en cours
    process_list[current_pid].state = LIBRE;
    //TODO: Changer le pid en cours
}

// Fonction pour créer un processus
int create_process(void* function) {
    pid_t pid;

    // On cherche un pid libre
    pid = find_free_pid();

    if (pid != 0)
        // La table des processus est pleine, on ne créer pas de processus
        return -1;
    else {

        // On initialise le processus
        process_list[pid].state = PRET;
        process_list[pid].pid = pid;
        process_list[pid].stack = (void*)0xA00000; //TODO: Mettre une adresse logique ,?

    return pid;
  }
}

void supress_process(pid_t pid) {
    // On libère le processus car le processus est terminé
    process_list[pid].state = LIBRE;
}

void block_process(pid_t pid) {
    // On bloque le processus
    process_list[pid].state = BLOQUE;
}

int sleep(int seconds) {
    // On bloque le processus pendant x seconds
    return 0;
}

void schedule() {

}
