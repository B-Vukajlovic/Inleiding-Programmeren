/* Naam: K. Klaassen
   UvAnetID: 12345678
   Studie: BSc Informatica
  
   Dit programma bevat verschillende functies waarmee de Collatz-reeks 
   onderzocht kan worden.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

int stoptijd;
int max;
int maxreeks;


int collatz_opvolger(int getal) {

    if (getal < 1 ) {
        return -1;        
    } else if (getal % 2 == 0) {
        getal = getal / 2;
        return getal;
    } else {
        getal = 3 * getal + 1;
        return getal;
    } 
}

int collatz_stoptijd(int reeks) {
    if(reeks < 1){
        return -1;
    }

    int stoptijd = 1;
    
    if (reeks % 2 == 1 && reeks > (INT_MAX - 1)/3) {
        return 0;
    }

    while (reeks > 1) {
        reeks = collatz_opvolger (reeks);
        stoptijd++;
    }
    return stoptijd;
}

int collatz_maximum(int reeks) {
    if(reeks < 1){
        return -1;
    }

    int max = reeks;
    while (reeks > 1) {
        reeks = collatz_opvolger (reeks);
        if (max < reeks) {
            max = reeks;
        }
    }
    return max;
}

void collatz_toon_reeks(int reeks) {
    if(reeks < 1){
        return -1;
    }
    
    printf("Reeks: %d", reeks);
    while (reeks > 1) {
        reeks = collatz_opvolger (reeks);
        printf(", %d", reeks);
    }
}

void collatz_info(int reeks) {

    if(collatz_opvolger(reeks) == -1) {
        printf("-1\n");
        return;
    }
    
    collatz_toon_reeks(reeks);
    printf("\nStoptijd: %d", collatz_stoptijd(reeks));
    printf("\nMaximum: %d\n", collatz_maximum(reeks));
    
}

int collatz_zoek_langste(int a, int b) {

    if(a < 1 || b < a){
        return -1;
    }
    int maxstoptijd = 0;

    for(a; a <= b; a++) {
        stoptijd = collatz_stoptijd(a);
        if (stoptijd > maxstoptijd) {
            maxstoptijd = stoptijd;
            maxreeks = a;
        } 
    
    }
    return maxreeks;
}


int collatz_zoek_grootste(int a, int b) {
    
    if(a < 1) {
        return -1;
    }

    if(b < 1) {
        return -1;
    }

    if(b < a) {
        return -1;
    }

    int maxwaarde;

    for(a; a <= b; a++) {
        max = collatz_maximum(a);
        if (max > maxwaarde) {
            maxwaarde = max;
            maxreeks = a;
        } 
    }
    return maxreeks;
}


void collatz_onderzoek(int a, int b) {
 
    if(a > 0 && b > 0 && b > a){
    printf("Langste Collatz reeks tussen %d en %d\n", a, b);
    collatz_toon_reeks(collatz_zoek_langste(a, b));
    printf("\nStoptijd: %d", collatz_stoptijd(collatz_zoek_langste(a, b)));
    printf("\nMaximum: %d\n\n", collatz_maximum(collatz_zoek_langste(a, b)));

    printf("Collatz reeks met de grootste waarde tussen %d en %d\n", a, b);
    collatz_toon_reeks(collatz_zoek_grootste(a, b));
    printf("\nStoptijd: %d", collatz_stoptijd(collatz_zoek_grootste(a, b)));
    printf("\nMaximum: %d\n", collatz_maximum(collatz_zoek_grootste(a, b)));
    } else {
        printf("-1\n");
    }
}

void collatz_incorrect(void) {
    int a = 1;
    
    for(a; a <= 1000000; a++) {
        int c = collatz_stoptijd(a);
        if (c == 0) {
            printf("%d\n", a);

        }
    }
}

#ifndef COMPILE_MAIN
int main(int argc, char **argv) {

    if (argc == 1) {
        // Programma is aangeroepen zonder argumenten
        collatz_incorrect();
    } else if (argc == 2) {
        // Programma is aangeroepen met 1 argument.
        collatz_info(atoi(argv[1]));
    } else if (argc == 3) {
        // Programma is aangeroepen met 2 argumenten.
        collatz_onderzoek(atoi(argv[1]), atoi(argv[2]));
    } else {
        printf("gebruik: opdracht2 [getal [tweede getal]]\n");
        return 1;
    }

    return 0;
}
#endif
