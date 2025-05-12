/** Interface du module de gestion du temps */

#ifndef TIME_H
#define TIME_H

#include <inttypes.h>


// Fonction d'initialisation de l'horloge
void init_clock();

// Fonction qui est executée à chaque interruption de l'horloge
void handler_clock();

// Affiche dasn le coin en haut à droite le compteur de l'horloge.
void afficher_compteur();

#endif