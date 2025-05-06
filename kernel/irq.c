#include <inttypes.h>
#include <n7OS/irq.h>


void init_irq_entry(int irq_num, uint32_t addr) {

    idt_entry_t* ligne_interuption = &idt[irq_num];

    ligne_interuption->zero = 0x0; // On complete juste de zéro
    ligne_interuption->offset_inf = addr & 0xFFFF; // Selection des 16 bits de poids faible
    ligne_interuption->offset_sup = (addr >> 16) & 0xFFFF; // Selection des 16 bits de poids fort
    ligne_interuption->sel_segment = KERNEL_CS; // Segment de code kernel
    ligne_interuption->type_attr = 0b10001110; // Type d'interruption (interrupt gate) = 14, P = 1 et S = 0
    
    // On remplit l'entrée de l'IDT
    //idt[irq_num] = &ligne_interuption;
    printf("adresse interup %x\n", idt[irq_num]);

}
