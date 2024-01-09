/* matching.c

   Naam: Boris Vukajlovic
   SID : 15225054
*/
// Dit programma speelt het spel steen, papier, schaar d.m.v patroon herkenning in voorgaande moves van zowel de tegenstander als jezelf. De arrays worden gevuld met waardes 0, 1 of 2 die respectievelijk voor steen, papier en schaar staan.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RONDES 1000
int lengte_patroon_f(int gelijk_pos, int tel_pos, int oude_gelijk_pos, int lengte_patroon, int keuze_i[], int keuze_t[]);

// Lengte_patroon_f is een functie die de lengte van het patroon voor een gegeven positie van de teller (tel_pos) en de gegeven positie van de match (gelijk_pos) binnen de arrays berekent
int lengte_patroon_f(int gelijk_pos, int tel_pos, int oude_gelijk_pos, int lengte_patroon, int keuze_i[], int keuze_t[]) {
    if (tel_pos > oude_gelijk_pos && gelijk_pos > 0) {
        if (keuze_i[tel_pos] == keuze_i[gelijk_pos] && keuze_t[tel_pos] == keuze_t[gelijk_pos]) {
            lengte_patroon++;
// D.m.v een recursie worden de positie van de teller (tel_pos) en de gegeven positie van de match (gelijk_pos) met 1 verminderd om zo het vorige getal in de arrays te checken voor een match
            return lengte_patroon_f(tel_pos - 1, gelijk_pos - 1, oude_gelijk_pos, lengte_patroon, keuze_i, keuze_t);
        }
    }
    return lengte_patroon;
}

void speel(void) {

    int MAX_INVOER = 1000;
    char buffer[MAX_INVOER];

    int keuze_i[1000];
    int keuze_t[1000];

    int langste_patroon = 0;
    int langste_patroon_pos = 0;
    int lengte_patroon = 0;
// De for loop incrementeerd de variabele tel_pos die dient als positie van het laatste getal in de arrays
    for (int tel_pos = 0; tel_pos < 1000; tel_pos++) {
// De eerste keer dat het programma wordt uitgevoerd zal er altijd als eerste papier geprint worden
        if (tel_pos < 1) {
            printf ("papier\n");
            keuze_i[tel_pos] = 1;
        }
// De for loop incrementeerd de variabele gelijk_pos die dient als positie van een match gevonden in de arrays
        for (int gelijk_pos = 0; gelijk_pos < tel_pos - 1; gelijk_pos++) {
// De positie van de match wordt gevonden in de arrays d.m.v de conditie in deze if statement
            if (keuze_i[gelijk_pos] == keuze_i[tel_pos - 1] && keuze_t[gelijk_pos] == keuze_t[tel_pos - 1]) {
                int oude_gelijk_pos = gelijk_pos;
// De functie voor het vinden van de lengte van het patroon wordt gevonden d.m.v de functie die hier geinitialiseerd wordt
                lengte_patroon = lengte_patroon_f(gelijk_pos, tel_pos - 1, oude_gelijk_pos, lengte_patroon, keuze_i, keuze_t);

                if (lengte_patroon > langste_patroon) {
                    langste_patroon = lengte_patroon;
                    langste_patroon_pos = gelijk_pos;
                }
            }
        }
// Aan de hand van het adres van het langst gevonden patroon wordt in deze if statements bepaald wat er gespeeld moet worden, dit gebeurt door bij de positie van de langste match 1 bij op te tellen en deze tegen te spelen
// Ook wordt hierbij de buffer van matching als speler (keuze_i) gevuld met de gekozen moves
        if (keuze_t[langste_patroon_pos + 1] == 2) {
            printf("steen\n");
            keuze_i[tel_pos] = 0;
        } else if (keuze_t[langste_patroon_pos + 1] == 1) {
            printf("schaar\n");
            keuze_i[tel_pos] = 2;
        } else if (keuze_t[langste_patroon_pos + 1] == 0) {
            printf("papier\n");
            keuze_i[tel_pos] = 1;
        }
// Stopt het voorgaande antwoord van de tegenstander in buffer
        char *resultaat = fgets(buffer, MAX_INVOER, stdin);
// De moves van de tegenstander worden met deze strcmp functies bepaald en in een array toegewezen
        if (strcmp(buffer, "schaar\n") == 0) {
            keuze_t[tel_pos] = 2;
        } else if (strcmp(buffer, "papier\n") == 0) {
            keuze_t[tel_pos] = 1;
        } else if (strcmp(buffer, "steen\n") == 0) {
            keuze_t[tel_pos] = 0;
        }
// Op het moment dat resultaat gelijk is aan NULL (niks) of stop zich in de buffer bevindt stopt het programma
        if (resultaat == NULL) {
            break;
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
