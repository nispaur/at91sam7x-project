
src/ - Sources du programme
=======================

# Utilisation
Placer dans ce dossier les sources du programme (fichiers *.c ou .S).

> **Note:**
>  Votre programme doit **toujours** contenir une fonction principale `main()`. 
>  Pour changer le nom de la fonction principale, éditer le branch final de fonction `_start` dans [`sys/reset.S`][reset] :

```asm
 _start:
    ; [...]
    str   r0, [r12, #AIC_ICCR]
    str   r0, [r12, #AIC_EOICR]
    
    ; Demasquer les interruptions
    mrs   r1, CPSR
    bic   r1, r1, #0x80
    msr   CPSR, r1
    
    ; Branchement vers la fonction principale
    b     main 
```

# Compilation

## Sources assembleur ARM (.S)

Par défaut, le Makefile compile et link toutes les sources assembleur (fichiers .S) présentes dans src/. Pour changer ce comportement, modifier les variables `ASRCARM` du [`Makefile`][makefile]:

```Makefile
# Fichier reset.S, indispensable au démarrage de la carte
ASRCARM = sys/reset.S

# Compile automatiquement tous les fichiers .S présents dans src/ (commenté ici)
# ASRCARM += $(wildcard src/*.S)

# Selection manuelle, ici uniquement src/{fileX,fileY,fileZ}.c
ASRCARM += $(addprefix src/, fileX.c fileY.c fileZ.c)
```

## Sources C

Idem, en éditant la variable `SRC` du [`Makefile`][makefile]:

```Makefile
# Compile automatiquement tous les fichiers .c dans src/
# SRC += $(wildcard src/*.c) 

# Selection manuelle, compile uniquement src/{fileX,fileY,fileZ}.c
SRC += $(addprefix src/, fileX.c fileY.c fileZ.c)
```

Une fois le [`Makefile`][makefile] configuré, compiler l'executable au format `.elf` avec la commande `make`

> Pour plus d'informations concernant la compilation et l'execution du programme, consulter le manuel utilisateur de GDB Dashboard.

[reset]: ../src/reset.S
[makefile]: ../Makefile
