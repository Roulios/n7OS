/**
 * @file paging.h
 * @brief Gestion de la pagination dans le noyau
 */
#ifndef _PAGING_H
#define _PAGING_H

#define PAGE_PRESENT 1
#define PAGE_RW      2
#define PAGE_USER    4
#define PAGE_DIRTY   16

#include <inttypes.h>

/**
 * @brief Description d'une ligne de la table de page
 * 
 */
typedef struct {
    uint32_t present: 1; // Present
    uint32_t write_acces: 1; // Droit d'écriture sur la page
    uint32_t user_mode: 1; // Page mode utilisateur ou noyau
    uint32_t accessed: 1; // Page accédée
    uint32_t dirty: 1; // Page modifiée
    uint32_t reserved: 4; // Bits réservés
    uint32_t avail: 3;
    uint32_t page: 20;
} page_table_entry_t;

/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur
 * mov 
 * orl $0x80000000, %eax
 * mov %eax, %cr0\n\t
 */
typedef union {
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // PTE = Page Table Entry 

/**
 * @brief Une table de page (PageTable) est un tableau de descripteurs de page
 * 
 */
typedef PTE * PageTable;

typedef struct {
        uint32_t present    : 1;   // Page table is present in memory
        uint32_t rw         : 1;   // Read-only if clear, readwrite if set
        uint32_t user       : 1;   // Supervisor level only if clear
        uint32_t reserved   : 9;   // Some useful bits not we dont use them for now
        uint32_t page_table : 20;  // Page table address
} page_dir_entry_t;

typedef union {
    page_dir_entry_t dir_entry;
    uint32_t value;
} PDE;

typedef PDE * PageDirectory;

/**
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 *        et active la pagination
 * 
 */
uint32_t initialise_paging();

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 * 
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
#endif