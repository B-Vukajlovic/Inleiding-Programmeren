/* spel.c

   Naam: Boris Vukajlovic
   SID : 15225054

   Dit programma bevat alle logica voor het spelen en het uiterlijk van het doolhofspel.
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rooster.h"

/*Kleurt characters een gewilde kleur

char c: Het desbetreffende charachter
rooster *rp: Een pointer naar het rooster

side effect: print characters in de terminal
*/
void colorize(rooster *rp, char c) {
    if (rooster_vraag_toestand(rp) == AAN_HET_SPELEN) {
        switch(c) {
            case '8':
                start_color();
                init_pair(1, COLOR_BLUE, COLOR_BLACK);
                attron(COLOR_PAIR(1));
                addch(c);
                attroff(COLOR_PAIR(1));
                break;
            case 'a':
                start_color();
                init_pair(1, COLOR_BLUE, COLOR_BLACK);
                attron(COLOR_PAIR(1));
                addch(c);
                attroff(COLOR_PAIR(1));
                break;
            case '$':
                start_color();
                init_pair(2, COLOR_GREEN, COLOR_BLACK);
                attron(COLOR_PAIR(2));
                addch(c);
                attroff(COLOR_PAIR(2));
                break;
            case '/':
                start_color();
                init_pair(4, COLOR_YELLOW, COLOR_BLACK);
                attron(COLOR_PAIR(4));
                addch(c);
                attroff(COLOR_PAIR(4));
                break;
            case 'X':
                start_color();
                init_pair(3, COLOR_RED, COLOR_BLACK);
                attron(COLOR_PAIR(3));
                addch(c);
                attroff(COLOR_PAIR(3));
                break;
            default:
                addch(c);
        }
    } else {
        addch(c);
    }
}

/* Toont het gegeven rooster met ncurses.

   rp: een pointer naar het rooster.
*/
void toon_rooster(rooster *rp) {
    clear();
    for (int y = 0; y < rooster_hoogte(rp); y++) {
        for (int x = 0; x < rooster_breedte(rp); x++) {
                char c = rooster_kijk(rp, x, y);
                colorize(rp, c);
        }
        addch('\n');
    }
}

/*Toont de handleiding van het spel
*/
void handleiding(void) {
    FILE *fh6 = fopen("assets/handleiding.txt", "r");
    if (fh6==NULL) {
        perror("main");
        exit(1);
    }

    rooster *rp6 = rooster_lees(fh6);
    fclose(fh6);

    toon_rooster(rp6);
    getch();
    rooster_klaar(rp6);

    clear();
}

/* Toont het startscherm van het spel
*/
void start_scherm(void) {
    FILE *fh1 = fopen("assets/intro.txt", "r");
        if (fh1==NULL) {
        perror("main");
        exit(1);
    }

    rooster *rp1 = rooster_lees(fh1);
    fclose(fh1);

    toon_rooster(rp1);
    getch();
    rooster_klaar(rp1);

    clear();

    FILE *fh2 = fopen("assets/gunman.txt", "r");
        if (fh2==NULL) {
        perror("main");
        exit(1);
    }

    rooster *rp2 = rooster_lees(fh2);
    fclose(fh2);

    toon_rooster(rp2);
    getch();
    rooster_klaar(rp2);
}

/*Toont een eindscherm wanneer de speler heeft verloren
*/
void verloren(void) {
    FILE *fh3 = fopen("assets/gunshot.txt", "r");
    if (fh3==NULL) {
        perror("main");
        exit(1);
    }

    rooster *rp3 = rooster_lees(fh3);
    fclose(fh3);

    toon_rooster(rp3);
    getch();
    rooster_klaar(rp3);

    clear();
}

/*Toont een eindscherm wanneer de speler heeft gewonnen

int info: Checkt of een bepaald object binne het spel is bemachtigd
*/
void gewonnen(int info) {
    if (info == 1) {
        FILE *fh4 = fopen("assets/gewonneninfo.txt", "r");
        if (fh4==NULL) {
            perror("main");
            exit(1);
        }

        rooster *rp4 = rooster_lees(fh4);
        fclose(fh4);

        toon_rooster(rp4);
        getch();
        rooster_klaar(rp4);

        clear();
    } else if (info == 0) {
        FILE *fh5 = fopen("assets/gewonnen.txt", "r");
        if (fh5==NULL) {
            perror("main");
            exit(1);
        }

        rooster *rp5 = rooster_lees(fh5);
        fclose(fh5);

        toon_rooster(rp5);
        getch();
        rooster_klaar(rp5);

        clear();
    }
}

/* Voert de benodigde veranderingen in het rooster door als de speler in een
   bepaalde richting probeert te bewegen.

   rp   : een pointer naar het rooster
   dx,dy: de richting waarin de speler probeert te bewegen. De vier mogelijk-
          heden voor (dx,dy) zijn (-1,0), (1,0), (0,-1), (0,1) voor resp.
          links, rechts, omhoog en omlaag.

   Side effect: het rooster wordt aangepast op basis van de handeling van
                de speler.
*/
void beweeg(rooster *rp, int dx, int dy, int *info) {
    int x, y;
    rooster_zoek(rp, '*', &x, &y);

    int posx = x + dx;
    int posy = y + dy;

    if (rooster_bevat(rp, posx, posy)) {
        char newPos = rooster_kijk(rp, posx, posy);

        switch (newPos) {
            case ' ':
                rooster_plaats(rp, x, y, ' ');
                rooster_plaats(rp, posx, posy, '*');
                break;
            case 'X':
                rooster_zet_toestand(rp, VERLOREN);
                break;
            case '/':
                rooster_zet_toestand(rp, GEWONNEN);
                break;
            case '$':
                rooster_plaats(rp, x, y, ' ');
                rooster_plaats(rp, posx, posy, '*');
                *info = 1;
            default:
                return;
        }
    }
}


/* Speelt het spel met een gegeven rooster tot de toestand niet langer
   AAN_HET_SPELEN is.

   rooster *rp = pointer naar het rooster rp
 */
void speel(rooster *rp) {
    handleiding();
    start_scherm();
    int info = 0;

    if (rooster_vraag_toestand(rp) == BEGIN) {
        rooster_zet_toestand(rp, AAN_HET_SPELEN);
    }

    while (rooster_vraag_toestand(rp) == AAN_HET_SPELEN) {

        toon_rooster(rp);

        int dx, dy;
        int key = getch();
        switch (key) {
            case KEY_UP:
                dx = 0; dy = -1;
                break;
            case KEY_DOWN:
                dx = 0; dy = 1;
                break;
            case KEY_LEFT:
                dx = -1; dy = 0;
                break;
            case KEY_RIGHT:
                dx = 1; dy = 0;
                break;
            default:
                dx = 0; dy = 0;
        }
        beweeg(rp, dx, dy, &info);

        if (rooster_vraag_toestand(rp) == VERLOREN) {
            clear();
            verloren();
        } else if (rooster_vraag_toestand(rp) == GEWONNEN) {
            clear();
            gewonnen(info);
        }
    }
}

int main(int argc, char *argv[]) {
    // 1. Controleer dat er een doolhofbestand is opgegeven op de command line.
    if (argc != 2) {
        fprintf(stderr, "gebruik: ./spel assets/voorbeeld_doolhof.txt\n");
        exit(1);
    }

    // 2. Open het doolhofbestand en lees het rooster.
    FILE *fh = fopen(argv[1], "r");
    if (fh==NULL) {
        perror("main");
        exit(1);
    }
    rooster *rp = rooster_lees(fh);
    fclose(fh);

    // 3. Bepaal of het lezen van het rooster is gelukt.
    if (rp==NULL) {
        fprintf(stderr, "Kan rooster niet maken.\n"); 
        exit(1); 
    }

    // 4. Initialiseer ncurses
    initscr();
    cbreak();             // zodat je kunt onderbreken met Ctrl+C
    keypad(stdscr, TRUE); // luister ook naar extra toetsen zoals pijltjes
    noecho();             // druk niet de letters af die je intypt

    // 5. Speel het spel.
    speel(rp);

    /* Kijkt naar de input en stuurt bijbehorende dx en dy coordinaten
    naar de beweeg functie */

    // 6. Sluit af.
    rooster_klaar(rp);
    endwin();
    return 0;
}
