#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>

uint16_t *scr_tab;
int cursor_pos= 0;
int max_value_cursor_pos = VGA_HEIGHT * VGA_WIDTH;

void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;
}

void scroll_ecran() {
    // on décale tout le contenu existant d'une ligne vers le haut
    memcpy(scr_tab, scr_tab + VGA_WIDTH,
            sizeof(uint16_t) * (VGA_HEIGHT - 1) * VGA_WIDTH);

    // effacer la nouvelle ligne
    memset(scr_tab + (VGA_HEIGHT - 1) * VGA_WIDTH,
            0, sizeof(uint16_t) * VGA_WIDTH);

    // WTF, TODO: Faire ça propre
    cursor_pos = (cursor_pos / VGA_WIDTH) * (VGA_WIDTH - 1) - 55;

    // Affichage du timer dans le coin à droite de l'écran
    afficher_compteur();
}

void set_cursor(int pos) {
    int poids_faible = pos % 256;
    int poids_fort = pos<<8;

    outb(CMD_HIGH, PORT_CMD); // Ecriture bit de poids fort
    outb(poids_fort, PORT_DATA);
    outb(CMD_LOW, PORT_CMD); // Ecriture bit de poids faible
    outb(poids_faible, PORT_DATA);
}

int get_cursor() {
    int pos = 0;
    outb(CMD_HIGH, PORT_CMD); // Lecture bit de poids fort
    pos = inb(PORT_DATA) << 8;
    outb(CMD_LOW, PORT_CMD); // Lecture bit de poids faible
    pos |= inb(PORT_DATA);  
    return pos;
}

void console_putchar(const char c) {

    // Gestion du scroll
    if(cursor_pos >= max_value_cursor_pos) {
        scroll_ecran();
        set_cursor(cursor_pos);
    }
    
    // Si c'est un caractère, on l'affiche
    if (c > 31 && c < 127) {
        scr_tab[cursor_pos]= (GREEN<<8)|c;
        cursor_pos = cursor_pos + 1;
    // Saut de ligne, retour colonne 0
    } else if (c == '\n') {
        cursor_pos = (cursor_pos / VGA_WIDTH)* VGA_WIDTH + VGA_WIDTH;

    // Décalage du curseur d'une colonne
    } else if (c == '\b') {

        // Vérif que l'on est pas au debut de la console, si c'est le cas on fait rien
        if(cursor_pos != 0) {
            cursor_pos = cursor_pos -1;
        }
    
    // Retour à la colonne 0 ligne courante
    } else if (c == '\r') {
        cursor_pos = (cursor_pos / VGA_WIDTH)* VGA_WIDTH;

    }
    // Décala
    else if(c == '\t') {
        cursor_pos = cursor_pos + 8;

    // Clear console
    } else if (c == '\f') {
        // On boucle sur tout les éléments du tableau de scr_tab pour mettre des espaces
        for(int i=0; (i< max_value_cursor_pos); i++) {
            scr_tab[i]= (CHAR_COLOR<<8)|' ';
        }

        cursor_pos = 0;
    }

    //Modification de l'affichage du curseur en fonction de son emplacement.
    set_cursor(cursor_pos);
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}

