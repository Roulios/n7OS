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

extern void handler_IT_clock();


// Fonction d'initialisation de l'horloge
void init_clock() {
    // Initialisation de la fréquence de l'horloge
    uint32_t freq = FREQ_OSCILLATEUR / HORLOGE;
    
    // Envoi de la commande pour initialiser le timer
    outb(0x34, 0x43); // Commande pour génération de l'inpulsion de l'horloge;
    
    // Envoi de la valeur du diviseur (16 bits)
    outb(freq & 0xFF, 0x40); // Octet de poids faible
    outb((freq >> 8) & 0xFF, 0x40); // Octet de poids fort

    outb(inb(0x21)&0xfe, 0x21); // Autorisation de l'interruption 0

    // Initialisation de l'interruption pour le timer (interruption 32)
    init_irq_entry(32, (uint32_t)handler_IT_clock);
}

void handler_clock() {

    //printf("Handler horloge\n");

    // Masquage de l'interruption 0
    outb(inb(0x21) | 0x01, 0x21); // Masquage de l'IRQ 0 TODO: Vérifier si c'est nécessaire

    // Incrémentation du nombre de ticks
    ticks++;
    outb(0x20, 0x20); // Envoi de l'acknowledge à l'IRQ 0
    outb(inb(0x21)&0xfe, 0x21); // Autorisation de l'interruption 0
    
    // Affichage du nombre de ticks sur la console
    if (ticks % 1000 == 0) {
        afficher_compteur();
    }
}

void afficher_compteur() {
    // Calcul des heures, minutes et secondes
    uint32_t seconds = ticks / 1000;
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;

    uint16_t pos = get_cursor();            // Récupération de la position actuelle du curseur
    set_cursor(VGA_WIDTH - 20);             // Déplacement du curseur en haut à droite

    printf("Uptime : %02d:%02d:%02d", hours % 24, minutes % 60, seconds % 60); // Affichage de l'heure

    set_cursor(pos);                        // Restauration de la position du curseur
}

// Fonction qui récupère le nombre de ticks pour convertir dans un format hh:mm:ss
/*
uint32_t get_ticks() {
    uint32_t ticks;
    __asm__ volatile("rdtsc" : "=A"(ticks));
    return ticks;
}*/
