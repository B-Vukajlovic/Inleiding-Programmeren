/* random.c

   Naam: Boris Vukajlovic
   SID : 15225054
*/
// Dit programma speelt steen, papier, schaar d.m.v het bijhouden van variabelen die aangeven hoe vaak de tegenspeler steen, papier of schaar heeft gebruikt, en op basis van deze gegevens tegenspeelt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void speel(void) {

    int const MAX_INVOER = 8;
    char buffer[MAX_INVOER];

    int steen_count = 0;
    int papier_count = 0;
    int schaar_count = 0;

    while(1) {
// Aan de hand van de grote van de variabelen steen_count, papier_count en schaar_count wordt bepaald welke move er door teller gespeeld wordt
        if (steen_count >= papier_count && steen_count >= schaar_count) {
            printf("papier\n");
        } else if (papier_count >= steen_count && papier_count >= schaar_count) {
            printf("schaar\n");
        } else if (schaar_count >= papier_count && schaar_count >= steen_count) {
            printf("steen\n");
        }
// Aan de hand van de variabelen steen_count, papier_count en schaar_count wordt bijgehouden hoe vaak de tegen stander steen, papier of schaar heeft gemoved
        if (strcmp(buffer, "steen\n") == 0) {
            steen_count++;
        } else if (strcmp(buffer, "papier\n") == 0) {
            papier_count++;
        } else if (strcmp(buffer, "schaar\n") == 0) {
            schaar_count++;
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
    speel();
    return 0;
}
