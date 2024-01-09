/* rooster.c

   Naam: Boris Vukajlovic
   SID : 15225054

   Dit programma verwerkt het doolhof uit het voorbeeld_doolhof.txt bestand, en initialiseert functies die data uit dit doolhof kunnen opvragen en verwerken.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "rooster.h"

// Definieert de maximale grote van de array
#define MAX_GROTE 1000

struct rooster_data {
   int breedte;
   int hoogte;
   char **roosterd;
   toestand toestand;
};

/*Berekent de lengte en de breedte en stopt ze in de struct waar pointer rp naar wijst

*rp: de struct pointer
*buffer: de char array waar fgets chars uit de datastream fh naartoe zal kopieren (doolhof)
*fh: de datastream die de chars uit het txt document bevat

Side effect: De hoogte en de breedte worden opgeslagen in de struct waar pointer rp naar wijst
*/
void lengte_breedte(rooster *rp, char *buffer, FILE *fh) {
   int breedte = 0;
   int hoogte = 0;

   while (fgets(buffer, MAX_GROTE, fh)) {
      breedte = strlen(buffer);
      hoogte++;
   }
   rp->breedte = breedte - 1;
   rp->hoogte = hoogte + 1;
}

/*Malloct de 2d array roosterd waar de struct pointer rp naartoe wijst en vult deze met de desbettreffende chars uit het .txt document (doolhof)

*rp: de struct pointer
*buffer: de char array waar fgets chars uit de datastream fh naartoe zal kopieren (doolhof)
*fh: de datastream die de chars uit het txt document bevat (doolhof)

Side effect: de roosterd member van de struct (2d array) waar pointer rp naar wijst wordt gevuld met chars uit het .txt document (doolhof)
 */
void rooster_vullen(rooster *rp, char *buffer, FILE *fh) {
   rp->roosterd = (char **)malloc(rp->hoogte * sizeof(char *));
   if (rp->roosterd == NULL) {
      perror("Memory allocation error");
      exit(1);
   }

   for (int i = 0; i < rp->hoogte; i++) {
      fgets(buffer, MAX_GROTE, fh);
      if (buffer == NULL) {
         perror("File read error");
         exit(1);
      }

      rp->roosterd[i] = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
      if (rp->roosterd[i] == NULL) {
         perror("Memory allcoation error");
         exit(1);
      }
      strcpy(rp->roosterd[i], buffer);
   }
}

/* Maak een rooster op basis van het gegeven bestand.

   fh: de stream waaruit het doolhof gelezen moet worden.

   Uitvoer: als alles goed gaat, een pointer naar een rooster (die op de heap is
            gealloceerd), dat overeenkomt met de gegeven beschrijving.
            De begintoestand is BEGIN.

            Als de beschrijving niet consistent is (bijvoorbeeld
            niet alle rijen zijn even lang, of er klopt iets anders niet), of
            als niet voldoende geheugen kan worden gereserveerd, dan wordt
            NULL teruggegeven. (In dat geval blijft geen gereserveerd geheugen
            achter.)
*/
rooster *rooster_lees(FILE *fh) {
   rooster *rp = malloc(sizeof(rooster));
   if (rp == NULL) {
      perror("Memory allocation error");
      exit(1);
   }
   rp->toestand = BEGIN;

   char buffer[MAX_GROTE];
   fgets(buffer, MAX_GROTE, fh);
   if (buffer == NULL) {
      perror("File not found");
      exit(1);
   }

   lengte_breedte(rp, buffer, fh);
   rewind(fh);
   rooster_vullen(rp, buffer, fh);

   return rp;

}

/* Sla een rooster op met de gegeven bestandsnaam in de subdirectory "assets".

   fh: de stream waar het doolhof heen geschreven moet worden.

   Side effect: het doolhof wordt naar de stream gestuurd.
*/
void rooster_schrijf(const rooster *rp, FILE *fh) {
}

/* Vraag de huidige toestand van het spel op.

   rp: een pointer naar het rooster.

   Uitvoer: de toestand (AAN_HET_SPELEN, GEWONNEN of VERLOREN).
*/
toestand rooster_vraag_toestand(const rooster *rp) {
   return rp->toestand;
}

/* Verander de huidige toestand van het spel.

   rp: een pointer naar het rooster.
   t:  de nieuwe toestand (AAN_HET_SPELEN, GEWONNEN of VERLOREN).
*/
void rooster_zet_toestand(rooster *rp, toestand t) {
   rp->toestand = t;
}

/* Geef alle resources vrij die zijn gealloceerd voor een rooster.

   rp: een pointer naar het rooster.
*/
void rooster_klaar(rooster *rp) {
   for(int i = 0; i < rp->hoogte; i++) {
      free(rp->roosterd[i]);
   }
   free(rp->roosterd);
   free(rp);
}

/* Geef de breedte van het rooster, dat wil zeggen, het aantal kolommen.

   rp: een pointer naar het rooster.

   Uitvoer: de breedte.
*/
int rooster_breedte(const rooster *rp) {
   return rp->breedte;
}

/* Geef de hoogte van het rooster, dat wil zeggen, het aantal rijen.

   rp: een pointer naar het rooster.

   Uitvoer: de hoogte.
*/
int rooster_hoogte(const rooster *rp) {
   return rp->hoogte;
}

/* Kijk of de gegeven positie binnen het rooster valt.

   rp:  een pointer naar het rooster.
   x,y: de positie.

   Uitvoer: 1 als de positie binnen het rooster valt, anders 0.
*/
int rooster_bevat(const rooster *rp, int x, int y){
   if (x >= 0 && x < rp->breedte && y >= 0 && y < rp->hoogte) {
      return 1;
   } else {
      return 0;
   }
}

/* Kijk welk object er staat op een bepaalde positie in het rooster.

   rp : een pointer naar het rooster
   x,y: de betreffende positie.

   Uitvoer: het object op die positie, of '\0' als de positie buiten het
            rooster valt.
*/
char rooster_kijk(const rooster *rp, int x, int y) {
   if (rooster_bevat(rp, x, y) == 1) {
      return rp->roosterd[y][x];
   } else {
      return '\0';
   }
}

/* Schrijf een bepaald object op een bepaalde positie in het rooster.

   rp : een pointer naar het rooster
   x,y: de betreffende positie.
   c  : het object.

   Side effect: als (x,y) binnen het rooster ligt, wordt het object op
                de opgegeven positie geplaatst, anders verandert er niets.

   Uitvoer: 1 als het object is geplaatst, of 0 als het buiten de grenzen lag.
*/
int rooster_plaats(rooster *rp, int x, int y, char c) {
   if (rooster_bevat(rp, x, y) == 1) {
      rp->roosterd[y][x] = c;
      return 1;
   } else {
      return 0;
   }
}

/* Zoek een bepaald object in het rooster, en geef de coordinaten van het
   object terug via de gegeven pointers. Let op: als er meerdere objecten van
   het gezochte soort in het rooster voorkomen, is niet gedefinieerd van welke
   de positie wordt gevonden.

   rp : een pointer naar het rooster
   c  : het object dat moet worden gezocht
   x,y: pointers naar de geheugenlocaties waar de gevonden positie moet worden
        geschreven.

   Uitvoer: via de pointers x en y. Als het object niet wordt gevonden worden
            *x en *y op -1 gezet.
*/
void rooster_zoek(const rooster *rp, char c, int *x, int *y) {
   *x = -1;
   *y = -1;

   for (int i = 0; i < rp->hoogte; i++) {
      for (int j = 0; j < rp->breedte; j++) {
         if (rp->roosterd[i][j] == c) {
               *y = i;
               *x = j;
               return;
         }
      }
   }
}
