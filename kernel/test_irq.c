#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>

extern void handler_IT();

// Variable temporaire pour tester l'interruption 50
#define INTERRUPTION_TEST 50

/** Initialisation du tableau des interruption*/
void init_irq() {
    // Init de l'interruption 50
    init_irq_entry(INTERRUPTION_TEST, (uint32_t)handler_IT);
}

/** Fonction handler des interruption en C, va permettre l'execution de commandes*/
void handler_en_C(int id_interruption) {
    // switch pour savoir quelle interruption on a
    switch (id_interruption) {
        case 32:
            // Gestion de l'interruption 32 = horloge
            handler_clock();
            break;
        default:
            printf("Reception de l'interruption : %i\n", id_interruption);
            break;
    }

}
