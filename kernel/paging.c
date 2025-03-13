#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL

uint32_t *page_directory = NULL;

const nb_pages = 1024;

/** initialiser une entrée de la table des pages avec les paramètres donnés */
void setPageEntry(PTE *page_table_entry, uint32_t new_page, int is_writeable, int is_kernel) {
  page_table_entry->page_entry.present= 1; // Page présente en mémoire
  page_table_entry->page_entry.reserved= 0; // Initialement non réservé
  page_table_entry->page_entry.dirty= 0; // Initialement non modifié
  page_table_entry->page_entry.write_acces= is_writeable; 
  page_table_entry->page_entry.user_mode= !is_kernel;
  page_table_entry->page_entry.page= new_page>>12; // Adresse de la page
}

void initialise_paging() {
    // Création du répertoire de pages.
    page_directory = kmalloc(nb_pages * sizeof(uint32_t)); // 1024 entrées de 32 bits dans le répertoire de pages.

    // Initialisation des entrées du répertoire de pages = les tables de pages.
    for (int i = 0; i < nb_pages; i++) {
        page_directory[i] = kmalloc(nb_pages * sizeof(PTE)); // 1024 entrées de 32 bits dans la table de pages.

    }

    // Actualisation de l'adresse de CR3
    __asm__ volatile("mov %0, %%cr3":: "r"(page_directory));

    

}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    PageTable pgtab= page_directory[address >> 22]; // On récupère l'adresse de la page de table dans le répertoire de page.

    // on recupere l'adresse de la page dans la table de page
    pgtab = (PTE*) (pgtab->page_entry.page << 12);

    // recherche d'une page libre dans la memoire physique pour la page de table
    uint32_t phy_page = findfreePage(); 

    // mise a jour de l'entree dans la page de table
    setPageEntry(&pgtab[0x3ff & (address >> 12)],
                phy_page, is_writeable, is_kernel);

    return pgtab;
}
