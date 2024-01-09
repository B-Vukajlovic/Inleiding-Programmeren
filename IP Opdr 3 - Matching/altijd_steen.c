/* altijd_steen.c

   Naam: Boris Vukajlovic
   SID : 15225054
*/
// Dit programma speelt steen, papier, schaar d.m.v altijd steen te kiezen als move
#include <stdio.h>
#include <string.h>

void speel(void) {

    int const MAX_INVOER = 8;
    char buffer[MAX_INVOER];

    while(1) {

        printf("steen\n");
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
