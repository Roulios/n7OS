.text
# cette directive sert a rendre l'etiquette publique
    .globl handler_IT_clock
# debut du traitant
handler_IT_clock:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
    call handler_clock
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret
