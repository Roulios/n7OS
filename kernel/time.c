/** module servant à la gestion de l'horloge */

#include <inttypes.h>
#include <n7OS/time.h>
#include <n7OS/cpu.h>
#include <n7OS/console.h>

// Fréquence de l'oscillateur (1.19 mHz)
#define FREQ_OSCILLATEUR 0x1234BD
#define HORLOGE 1000

// Nombre de ticks de l'horloge actuel.
static uint32_t ticks = 0;

// Fonction d'initialisation de l'horloge
void init_clock() {
    // Initialisation de la fréquence de l'horloge
    uint32_t freq = FREQ_OSCILLATEUR / HORLOGE;
    
    // Envoi de la commande pour initialiser le timer
    outb(0x34, 0x43); // Commande pour génération de l'inpulsion de l'horloge;
    
    // Envoi de la valeur du diviseur (16 bits)
    outb(freq & 0xFF, 0x40); // Octet de poids faible
    outb((freq >> 8) & 0xFF, 0x40); // Octet de poids fort
}

void handler_clock() {
    // Affichage du nombre de ticks sur la console
    if (ticks % 1000 == 0) {
        afficher_compteur();
    }
}

void afficher_compteur() {
    uint16_t pos = get_cursor();            // Récupération de la position actuelle du curseur
    set_cursor(VGA_WIDTH - 20);             // Déplacement du curseur en haut à droite
    printf("uptime: %ds", ticks / 1000);
    set_cursor(pos);                        // Restauration de la position du curseur
}

// Fonction qui récupère le nombre de ticks pour convertir dans un format hh:mm:ss
uint32_t get_ticks() {
    uint32_t ticks;
    __asm__ volatile("rdtsc" : "=A"(ticks));
    return ticks;
}
