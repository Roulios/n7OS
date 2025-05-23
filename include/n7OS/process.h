/**
 * @file process.h
 * @brief Gestion des processus
 */


#ifndef _PROCESS_H
#define _PROCESS_H

#include <inttypes.h>

typedef uint32_t pid_t;

/*
    * @brief État d'un processus
    * 
    * ELU: En cours d'exécution
    * PRET: Prêt à être exécuté
    * BLOQUE: Bloqué en attente d'un événement
    * LIBRE: Libéré
*/
typedef enum { ELU, PRET, BLOQUE, LIBRE } process_state_t;

typedef struct {
    void* stack;                // Pointeur vers la pile du processus
    uint32_t pid;               // Identifiant du processus
    process_state_t state;      // État du processus
    //TODO: Ajouter d'autres champs nécessaires pour les ressources ?
} process_t;

/**
 * @brief Crée un processus
 * 
 * @param function Fonction à exécuter par le processus
 * @return pid_t Identifiant du processus créé
 */
int create_process(void* function);

/**
 * @brief Supprime un processus
 * 
 * @param pid Identifiant du processus à supprimer
 */
void supress_process(pid_t pid);

void block_process(pid_t pid);


//pid_t fork(const char *name, fnptr function);

// Quitte le processus en cours
int exit();

// Récupère le pid du processus en cours
pid_t getpid();

// Met en pause le processus en cours pendant un certain temps
int sleep(int seconds);

// Fonction d'ordonnancement des processus
void schedule();


#endif