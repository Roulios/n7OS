#include <n7OS/keyboard.h>
#include <n7OS/cpu.h>
#include <n7OS/console.h>


extern void handler_IT_keyboard();

// Buffer circulant pour stocker les caractères
#define BUFFER_CIRCULAIRE_SIZE 256
char buffer_char[BUFFER_CIRCULAIRE_SIZE];

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
    if(!(code & 0x80)) {
        // La touche est pressée, on va donc l'écrire dans le terminal, on doit convertir le code en un caractère

        // Conversion du code en caractère a=0x10,TODO: Vérifier si c'est le bon code
        char c = 0;
        if(code >= 0x10 && code <= 0x19) {
            c = 'a' + (code - 0x10);
        } else if(code >= 0x1A && code <= 0x23) {
            c = 'A' + (code - 0x1A);
        } else if(code >= 0x2C && code <= 0x35) {
            c = '0' + (code - 0x2C);
        } else if(code == 0x29) {
            c = ' ';
        } else if(code == 0x1C) {
            c = '\n';
        }

        kgetch();
    }
}



// Fonction qui retourne un caractère stocké dans buffer circulant de taille connue
char kgetch() {
    // On va lire le buffer circulant
    int buffer_char_index = 0;

    char c = 0;
    if(buffer_char != 0) {
        c = buffer_char[buffer_char_index];
        buffer_char_index++;
    }

    return c;
}