#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <n7OS/irq.h>
#include <n7OS/time.h>
#include <unistd.h>
#include <n7OS/sys.h>

extern void handler_IT();

void kernel_start(void)
{
    uint32_t pagedir;
    init_console();
    pagedir= initialise_paging();
    setup_base(pagedir /* la memoire virtuelle n'est pas encore definie */);

    init_syscall();

    // Initialisation de l'horloge
    init_clock();

    // Adresse totalement aléatoire pour l'interruption 50
    init_irq_entry(50, (uint32_t)handler_IT);

    // lancement des interruptions
    sti();

    //__asm__ volatile ("int $50"::); // On lance l'interruption 50

    for(int i = 0; i < 100; i++) {
        printf("Hello world ! %d\n", i);
    }
    

    // on ne doit jamais sortir de kernel_start
    while (1) {    
        alloc_page_entry(0xA00000, 1, 0);
        int* pipi = (int*)0xA00000;
        *pipi = 0x12345678;
        hlt();
    }
}
