/* random.c

   Naam: Boris Vukajlovic
   SID : 15225054
*/
// Dit programma speelt steen, papier, schaar d.m.v een willekeurige keuze tussen steen, papier of schaar.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void speel(void) {

    int const MAX_INVOER = 8;

    char buffer[MAX_INVOER];

    while(1) {
// Aan de hand van de rand() functie wordt een willekeurig getal tot de 3 gekozen (0, 1 of 2) en in een variabele gestopt
        int random = rand() % 3;
// Aan de hand van de random variabele wordt nu een keuze gemaakt tussen steen, papier of schaar
        if (random == 0) {
            printf("steen\n");
        } else if (random == 1) {
            printf("schaar\n");
        } else if (random == 2) {
            printf("papier\n");
        }
// Stopt het voorgaande antwoord van de tegenstander in buffer
        char *resultaat = fgets(buffer, MAX_INVOER, stdin);
// Op het moment dat resultaat gelijk is aan NULL (niks) of stop zich in de buffer bevindt stopt het programma
        if (resultaat == NULL) {
            return;
        } else if (strcmp(buffer, "stop\n") == 0) {
            break;
        }
    }
}

int main(void) {
    setlinebuf(stdout);
    srandom(69420789);
    speel();
    return 0;
}
