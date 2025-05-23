#include <n7OS/keyboard.h>
#include <n7OS/cpu.h>
#include <n7OS/console.h>


extern void handler_IT_keyboard();

// Buffer circulant pour stocker les caractères
#define BUFFER_CIRCULAIRE_SIZE 256
char buffer_char[BUFFER_CIRCULAIRE_SIZE];

int* buffer_index_start;
int* buffer_index_end;

*buffer_index_end = 0;
*buffer_index_start = 0;

// Fonction d'initialisation des input de la part d'un clavier
void init_keyboard() {

    // Démasquage de l'interruption clavier
    outb(inb(0x21)&0xfd, 0x21);

    // Initialisation de l'interruption pour l'input clavier (interruption 33)
    init_irq_entry(33, (uint32_t)handler_IT_keyboard);
}

void handler_keyboard() {
    // Masquage de l'interruption clavier
    outb(inb(0x21) | 0x02, 0x21); // Masquage de l'IRQ 1 TODO: Vérifier si c'est nécessaire

    // Envoi de l'acknowledge à l'IRQ1
    outb(0x20, 0x20);
    outb(inb(0x21)&0xfd, 0x21); // Autorisation de l'interruption

    // Lecture du code de la touche
    uint8_t code = inb(0x60);

    // Vérification si la touche est pressée ou relâchée
    if(!IS_KEY_RELEASED(code)) {
        char c = 0;

        // La touche est pressée, on va donc l'écrire dans le terminal, on doit convertir le code en un caractère
        if(code < 0x80) {
            // On va chercher le code dans le tableau de conversion
            c = scancode_map[code];
        } else {
            // On va chercher le code dans le tableau de conversion avec shift
            c = scancode_map_shift[code - 0x80];
        }        

        // Ajout de la touche dans le buffer circulant
        buffer_char[*buffer_index_end] = c;
        *buffer_index_end += 1;

        // Si on a dépassé la taille du buffer, on revient au début
        if(*buffer_index_end >= BUFFER_CIRCULAIRE_SIZE) {
            *buffer_index_end = 0;
        }
    }
}


// Fonction qui retourne un caractère stocké dans le buffer circulant
char kgetch() {
    char c = buffer_char[*buffer_index_start];
    *buffer_char += 1;

    // Si on a dépassé la taille du buffer, on revient au début
    if(*buffer_index_start >= BUFFER_CIRCULAIRE_SIZE) {
        *buffer_index_start = 0;
    }

    return c;
}