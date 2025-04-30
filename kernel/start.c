#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>

void kernel_start(void)
{
    uint32_t pagedir;
    init_console();
    pagedir= initialise_paging();
    setup_base(pagedir /* la memoire virtuelle n'est pas encore definie */);

    // lancement des interruptions
    sti();

    // on initialise la pagination
    

    // on ne doit jamais sortir de kernel_start
    while (1) {
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\t fezfez   e\nhyhiu");
    
        alloc_page_entry(0xA00000, 1, 0);
        int* pipi = (int*)0xA00000;
        *pipi = 0x12345678;
        hlt();
    }
}
