#include <n7OS/mem.h>
#include <n7OS/kheap.h>

#define NB_PAGES ((LAST_MEMORY_INDEX+1)/PAGE_SIZE) // nombre de pages en mémoire.
#define SIZE (NB_PAGES/32)

/* Pas utile, c'était pour la mémoire virtuelle, sinj
typedef struct {
    uint8_t present: 1; // Present
    uint8_t write_acces: 1; // Droit d'écriture sur la page
    uint8_t user_mode: 1; // Page mode utilisateur ou noyau
    uint8_t accessed: 1; // Page accédée
    uint8_t dirty: 1; // Page modifiée
    uint8_t reserved_bits: 4; // Bits réservés
    uint8_t avail_bit: 3;
    uint32_t page: 20;
} page_table_t;
*/
 
uint32_t free_page_bitmap_table[SIZE];

/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    int index = addr / PAGE_SIZE; // Index de l'element de la bitmap qu'on va modifier
    int bit = addr % PAGE_SIZE; // Bit de l'element de la bitmap qu'on va modifier
    free_page_bitmap_table[index]|(1 << bit);

}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    int index = addr % PAGE_SIZE; // Index de l'element de la bitmap qu'on va modifier
    int bit = addr / PAGE_SIZE; // Bit de l'element de la bitmap qu'on va modifier
    free_page_bitmap_table[index]|(0 << bit);
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
    uint32_t adresse= 0x0;
    for(int i=adresse; i<SIZE; i++) {

        //printf("Bitmap à l'étape %i :\n", i);

        if (free_page_bitmap_table[i] != 0xFFFFFFFF) { // Si la bitmap n'est pas rempli de 0, alors on a une page libre
            for(int j=0; j<32; j++) {
                if ((free_page_bitmap_table[i] & (1 << j)) == 0) { // Si le bit est à 0, alors la page est libre
                    //free_page_bitmap_table[i]|(1 << j);
                    adresse = (i*32 + j)*PAGE_SIZE;

                    free_page_bitmap_table[i] |= (1 << j);

                    // On retourne l'adresse de la page trouvé et on sort de la boucle.
                    return adresse; 
                }
            }
            
        }
    }
    return adresse;
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    // Initialisation de la tête du tas
    kheap_init();

    for(int i=0; i<SIZE; i++) {
        free_page_bitmap_table[i] = 0;
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    for(int i=0; i<SIZE; i++) {
        printf("Page %d : %d\n", i, free_page_bitmap_table[i]);
    }
}