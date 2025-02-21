#include <n7OS/console.h>

uint16_t *scr_tab;
int cursor_pos= 0;

void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;
}

void console_putchar(const char c) {
    // Si c'est un caractère, on l'affiche
    if (c > 31 && c < 127) {
        scr_tab[cursor_pos]= (PURPLE<<8)|c;
        cursor_pos = cursor_pos + 1;
    // Saut de ligne, retour colonne 0
    } else if (c == '\n') {
        cursor_pos = (cursor_pos / 80)* 80 + 80;

    // Décalage du curseur d'une colonne
    } else if (c == '\b') {

        // Vérif que l'on est pas au debut de la console, si c'est le cas on fait rien
        if(cursor_pos != 0) {
            cursor_pos = cursor_pos -1;
        }
    
    // Retour à la colonne 0 ligne courante
    } else if (c == '\r') {
        cursor_pos = (cursor_pos / 80)* 80;

    }
    else if(c == '\t') {
        cursor_pos = cursor_pos + 8;

    // Clear console
    } else if (c == '\f') {
        // On boucle sur tout les éléments du tableau de scr_tab pour mettre des espaces
        for(int i=0; (i< 80*25); i++) {
            scr_tab[i]= (CHAR_COLOR<<8)|' ';
        }

        cursor_pos = 0;
    }
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}

