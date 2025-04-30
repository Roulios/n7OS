#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL
#include <n7OS/mem.h>
#include <n7OS/kheap.h>
#include <stdio.h>

const nb_pages = 1024;

PageDirectory pageDirectory = NULL;

int find = 0;


/** initialiser une entrée de la table des pages avec les paramètres donnés */
void setPageEntry(PTE *page_table_entry, uint32_t new_page, int is_writeable, int is_kernel) {
/*
  page_table_entry->page_entry.present= 1; // Page présente en mémoire
  page_table_entry->page_entry.reserved= 0; // Initialement non réservé
  page_table_entry->page_entry.dirty= 0; // Initialement non modifié
  page_table_entry->page_entry.write_acces= is_writeable; 
  page_table_entry->page_entry.user_mode= !is_kernel;
  page_table_entry->page_entry.accessed= 0;
  page_table_entry->page_entry.page= new_page>>12; // Adresse de la page
*/
  page_table_entry->value= new_page|PAGE_PRESENT|((is_writeable)?PAGE_RW:0)|((!is_kernel)?PAGE_USER:0);

  //printf("page entry set %x \n\n", page_table_entry->value);
}

uint32_t initialise_paging() {

    // Initialisation de la mémoire
    init_mem();

    uint32_t index= 0;

    // Création du répertoire de pages.
    pageDirectory= (PageDirectory) kmalloc_a (sizeof(PDE)*1024);
    memset(pageDirectory, 0, sizeof(PDE)*1024);
    
    for (int i= 0; i<1024; ++i) {
        PageTable new_page_table= (PageTable) kmalloc_a(sizeof(PTE)*1024);
        memset(new_page_table, 0, (sizeof(PTE)*1024));
        pageDirectory[i].value= (uint32_t) new_page_table|PAGE_PRESENT|PAGE_RW;
        index= (uint32_t) new_page_table + sizeof(PTE) * 1024;
    }

    

    for (int i= 0; i<index; i += PAGE_SIZE) {
        alloc_page_entry(i, 1, 1);
    }

    // Actualisation de l'adresse de CR3
    __asm__ volatile("mov %0, %%cr3":: "r"(pageDirectory));

    // Activation de la pagination
    __asm__ volatile("mov %cr0, %eax; orl $0x80000000, %eax; mov %eax, %cr0\n\t");

    return (uint32_t) pageDirectory;
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    PageTable pgtab;
    
    PDE page_directory_entry = pageDirectory[(address >> 22)];

    // on recupere l'adresse de la page dans la table de page
    pgtab = (PTE*) (page_directory_entry.dir_entry.page_table << 12);

    // recherche d'une page libre dans la memoire physique pour la page de table
    uint32_t phy_page = findfreePage(); 

   // printf("phy_page %x\n", phy_page);

    // mise a jour de l'entree dans la page de table
    setPageEntry(&pgtab[0x3ff & (address >> 12)],
                phy_page, is_writeable, is_kernel);

    
    printf("%x  à l'adresse : %x\n", pgtab[0x3ff & (address >> 12)].value,  address);
  

    return pgtab;
}
