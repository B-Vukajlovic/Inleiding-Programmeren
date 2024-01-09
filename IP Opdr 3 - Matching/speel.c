/* De jury voor de steen-papier-schaarcompetitie.

   Geschreven door Steven de Rooij in 2023.
*/


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#define MAX_SPELERS 200
#define MAX_NAMEN (MAX_SPELERS * 20)
#define MAX_PATH 1000
#define MAX_BUFFER 100

#define RONDES 1000
#define MARGE  33

typedef enum { STEEN, PAPIER, SCHAAR, STOP } keuze;

/* Struct speler houdt alle informatie over een speler bij. */
struct speler {
    const char *naam;    // De bestandsnaam van de speler (zonder directory)
    FILE *naar_speler;   // Output stream naar deze speler
    FILE *van_speler;    // Input stream van deze speler
    pid_t proces;        // Het proces id van deze speler
    int win;             // Hoe vaak deze speler heeft gewonnen,
    int verlies;         //   verloren en gelijk gespeeld in de
    int gelijk;          //   competitle.
    int id;              // Every player gets their own id so they don't get lost
                         //   when the results are sorted.
};


/* Initialiseert een struct_speler op basis van de bestandsnaam van een speler.

   sp:   een pointer naar de struct speler die moet worden geinitialiseerd.
   naam: de bestandsnaam van de betreffende executable.
*/
void init_speler(struct speler *sp, const char *naam, int id) {
    sp->naam = naam;
    sp->naar_speler = NULL;
    sp->van_speler = NULL;
    sp->proces = 0;
    sp->win = 0;
    sp->verlies = 0;
    sp->gelijk = 0;
    sp->id = id;
}

/* Stopt een speler door er de opdracht "stop" heen te sturen en daarna het
   proces en de streams te sluiten.

   sp: een pointer naar de betreffende struct speler.
*/
void stop_speler(struct speler *sp) {
    int stat_loc;
    assert(sp->proces);
    fprintf(sp->naar_speler, "stop\n");
    fclose(sp->naar_speler);
    fclose(sp->van_speler);

    pid_t pid = waitpid(sp->proces, &stat_loc, 0); // wacht tot het proces stopt
    if (pid == -1) {
        perror("Stoppen van speler mislukt");
        exit(1);
    }
    sp->naar_speler = NULL;
    sp->van_speler = NULL;
    sp->proces = 0;
}


/* Start een speler. Dit betekent dat de executable wordt opgestart in een nieuw
   proces dat wordt aangemaakt met de system call fork(). Met pipe() worden 
   communicatiekanalen gemaakt om met de speler te communiceren.
   Al deze informatie wordt in de struct speler opgeslagen.

   sp:        de speler die moet worden opgestart.
   directory: de directory waarin de speler sp->naam kan worden gevonden.
*/
void start_speler(struct speler *sp, const char *directory) {
    char path[MAX_PATH];
    if (snprintf(path, MAX_PATH, "%s/%s", directory, sp->naam) == MAX_PATH) {
        fprintf(stderr, "Te lange bestandsnaam: %s/%s\n", directory, sp->naam);
        exit(1);
    }

    int naar_speler[2], naar_manager[2];
    if (pipe(naar_speler) == -1 || pipe(naar_manager) == -1) {
        perror("Pipes maken mislukt");
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork mislukt");
        exit(1);
    }

    if (pid == 0) {
        // kindproces
        close(naar_speler[1]);
        dup2(naar_speler[0], 0);
        close(naar_manager[0]);
        dup2(naar_manager[1], 1);
        if (execlp(path, sp->naam, NULL) == -1) {
            perror("Starten van de speler is mislukt");
            exit(1);
        }
    } 

    sp->proces = pid;

    // terug in het ouderproces
    close(naar_speler[0]);
    sp->naar_speler = fdopen(naar_speler[1], "w");
    setlinebuf(sp->naar_speler);
    
    close(naar_manager[1]);
    sp->van_speler = fdopen(naar_manager[0], "r");
    setlinebuf(sp->van_speler);
}


/* Initialiseert een struct speler voor alle spelers in een gegeven directory.

   directory: de naam van de directory waar de spelers moeten worden gezocht.
   spelers:   het array waar alle struct spelers moeten worden opgeslagen.
   buffer:    een buffer die wordt gebruikt om de namen van alle spelers in
              op te slaan. De buffer moet blijven bestaan zolang de spelers
              nog gebruikt worden.

    Uitvoer: het aantal spelers dat is gevonden.
*/
int lees_spelers(const char *directory, struct speler *spelers, char *buffer) {
    DIR *dh = opendir(directory);
    if (dh == NULL) {
        perror("lees_spelers"); 
        exit(1);
    }
    const char *buffer_eind = buffer + MAX_NAMEN;
    int aantal_spelers = 0;
    while (1) {
        struct dirent *de = readdir(dh);
        if (de == NULL) { 
            break; 
        }
        #ifdef _DIRENT_HAVE_D_NAMLEN
        char *nieuw = buffer + de->d_namlen + 1;
#else  
        char *nieuw = buffer + strlen(de->d_name) + 1;
#endif
        if (nieuw > buffer_eind) {
            fprintf(stderr, "De namen van de spelers passen niet allemaal in de buffer.\n");
            exit(1);
        }
        if (de->d_type == DT_REG || de->d_type == DT_LNK) {
            strcpy(buffer, de->d_name);
            init_speler(&spelers[aantal_spelers], buffer, aantal_spelers);
            aantal_spelers++;
            buffer = nieuw;
        }
    }
    return aantal_spelers;
}


keuze wint_van(keuze k) { 
    return k == STEEN ? PAPIER : k == PAPIER ? SCHAAR : STEEN;
}

keuze verliest_van(keuze k) {
    return k == STEEN ? SCHAAR : k == PAPIER ? STEEN : PAPIER; 
}

const char *woord(keuze k) {
    if (k==STEEN)       { return "steen";  }
    else if (k==PAPIER) { return "papier"; }
    else if (k==SCHAAR) { return "schaar"; }
    else                { return "stop";   }
}

keuze luister_stream(FILE *fh) {
    static char buffer[MAX_BUFFER];
    if (fgets(buffer, MAX_BUFFER, fh) == NULL) {
        return STOP;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';

    if      (strcmp(buffer, "steen")==0)  { return STEEN;  }
    else if (strcmp(buffer, "papier")==0) { return PAPIER; }
    else if (strcmp(buffer, "schaar")==0) { return SCHAAR; }

    return STOP;
}

/* Voert een keuze uit naar de juiste stream.

   k: de betreffende keuze.
   fh: de stream.
*/
void spreek_stream(keuze k, FILE *fh) {
    fprintf(fh, "%s\n", woord(k));
}

/* Speelt het spel voor RONDES rondes tussen twee gegeven spelers in de gegeven
   directory. De spelers moeten geinitialiseerd zijn, maar nog niet gestart.

   directory: de naam van de directory waar de spelers staan opgeslagen.
   sp1: een pointer naar de eerste speler
   sp2: een pointer naar de tweede speler.
   verbose: 0 als geen uitvoer moet worden getoond
            1 als de resultaten van elke ronde moeten worden getoond.
*/
void speel(const char *directory, struct speler *sp1, struct speler *sp2, int verbose) {
    
    start_speler(sp1, directory);
    start_speler(sp2, directory);

    int score[2] = {0, 0};


    for (int ronde = 1; ronde <= RONDES; ronde++) {
        keuze k1 = luister_stream(sp1->van_speler);
        keuze k2 = luister_stream(sp2->van_speler);
        if      (wint_van(k1) == k2) { score[1] += 2; }
        else if (wint_van(k2) == k1) { score[0] += 2; }
        else { score[0] += 1; score[1] += 1; }
        if (verbose) {
            printf("Ronde %3d: %s zegt %s, %s zegt %s. Score wordt %d-%d.\n", 
                ronde,
                sp1->naam, woord(k1),
                sp2->naam, woord(k2),
                score[0], score[1]);
        }
        if (ronde == RONDES) { 
            break; 
        }
        spreek_stream(k2, sp1->naar_speler);
        spreek_stream(k1, sp2->naar_speler);
    }

    stop_speler(sp1);
    stop_speler(sp2);

    if (abs(score[0] - score[1]) < MARGE) {
        sp1->gelijk++; sp2->gelijk++;
    } else if (score[0] < score[1]) {
        sp1->verlies++; sp2->win++;
    } else {
        sp1->win++; sp2->verlies++;
    }

}

/* Een hulpfunctie die wordt gebruikt om spelers in een competitie te
   sorteren op basis van hun resultaten, zodat die in de juiste volgorde
   kunnen worden afgedrukt.

   sp1v: een void pointer naar de eerste speler (moet nog worden gecast).
   sp2v: een void pointer naar de tweede speler (moet nog worden gecast).
*/
int vergelijk_spelers(const void *sp1v, const void *sp2v) {
    const struct speler *sp1 = (const struct speler *)sp1v;
    const struct speler *sp2 = (const struct speler *)sp2v;
    return (2*sp2->win + 1*sp2->gelijk) - (2*sp1->win + 1*sp1->gelijk);
}

/* Houdt een competitie tussen alle spelers in de opgegeven directory,
   en drukt de resultaten af in de vorm van een tabel.

   directory: de directory waar alle spelers moeten worden gezocht.
*/
void competitie(const char *directory) {
    struct speler spelers[MAX_SPELERS];
    char buffer[MAX_BUFFER];
    int n = lees_spelers(directory, spelers, buffer);
    int resultaten[n][n];
    for (int i=0; i<n; i++) { 
        for (int j=0; j<n; j++) {
            resultaten[i][j] = 1;
        }
    }
    printf("%d spelers\n", n);
    for (int i=1; i<n; i++) {
        for (int j=0; j<i; j++) {
            struct speler sp1_kopie = spelers[i], sp2_kopie = spelers[j];
            speel(directory, &spelers[i], &spelers[j], 0);
            resultaten[i][j] = 2*(spelers[i].win - sp1_kopie.win) + 1*(spelers[i].gelijk - sp1_kopie.gelijk);
            resultaten[j][i] = 2*(spelers[j].win - sp2_kopie.win) + 1*(spelers[j].gelijk - sp2_kopie.gelijk);
        }
    }
    qsort(spelers, (size_t)n, sizeof(struct speler), &vergelijk_spelers);

    for (int j=100; j>=1; j/=10) {
        if (j>1) {
            printf("                               ");
        } else {
            printf("Speler                 +  =  -  ");
        }
        for (int i=1; i<=n; i++) {
            if (i >= j) {
                putchar('0' + ((i/j)%10));
            } else {
                putchar(' ');
            }
        }
        printf("\n");
    }
    
    printf("--------------------------------");
    for (int i=0; i<n; i++) { 
        putchar('-'); 
    }
    printf("\n");
    for (int i=0; i<n; i++) {
        printf("%3d. %-15s: %2d %2d %2d  ", i+1, spelers[i].naam, spelers[i].win, spelers[i].gelijk, spelers[i].verlies);
        int id_i = spelers[i].id;
        for (int j=0; j<n; j++) {
            int id_j = spelers[j].id;
            putchar("-=+"[resultaten[id_i][id_j]]);
        }
        printf("\n");
    }
}

/* Dit programma voert ofwel een wedstrijd uit tussen twee opgegeven spelers,
   of een hele competitie gegeven een directory met meerdere spelers.
*/
int main(int argc, const char **argv) {
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "gebruik: speel <directory> of speel <directory> <speler1> <speler2>\n");
        exit(1);
    }
    if (argc == 4) {
        struct speler sp1, sp2;
        init_speler(&sp1, argv[2], 0);
        init_speler(&sp2, argv[3], 1);

        speel(argv[1], &sp1, &sp2, 1);
    } else if (argc == 2) { 
        competitie(argv[1]);
    }
    return 0;
}
