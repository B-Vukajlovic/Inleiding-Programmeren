/* Naam: Boris Vukajlovic
   UvAnetID: 15225054
   Studie: BSc Informatica

   Dit programma implementeert de Caesar en Vigenere geheimschriften, en code
   om cryptogrammen te kraken.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_GROOTTE 500000
#define MAX_SLEUTEL 16
// Lees de gegeven voorbeelden en verwerkt ze in arrays
void lees_bestand(const char *fn, char *str) {
    FILE *fh = fopen(fn, "r");
    if (fh==NULL) {
        perror("Kan invoer niet lezen");
        exit(EXIT_FAILURE);
    }
    size_t grootte = fread(str, 1, MAX_GROOTTE-1, fh);
    fclose(fh);
    if (grootte==MAX_GROOTTE-1) {
        fprintf(stderr, "De invoer was te lang.\n");
        exit(1);
    }
    str[grootte] = '\0';
}
// Versimpelt de invoer door spaties, hoofdletter en leestekens weg te halen
void versimpel(char *str) {
    int input_pos = 0;
    int output_pos = 0;

    while (str[input_pos] != '\0') {
        if (isupper(str[input_pos])) {
            str[output_pos] = (char) tolower(str[input_pos]);
            output_pos++;
        } else if (isalpha(str[input_pos])) {
            str[output_pos] = str[input_pos];
            output_pos++;
        }
        input_pos++;
    }
    str[output_pos] = '\0';
}
// Deelfunctie die tekst versleuteld naar caesar
void caesar_versleutel(char *bericht) {
    for (int i = 0; bericht[i] != '\0'; i++) {
        char character = bericht[i];
            if (character <= 119) {
                bericht[i] = (char)(character + 3);
            } else {
                bericht[i] = (char)(character - 23);
            }
    }
}
// Versleuteld tekst naar caesar
void opdracht_caesar_versleutel(const char *fn) {
    char bericht[MAX_GROOTTE];

    lees_bestand(fn, bericht);
    versimpel(bericht);
    caesar_versleutel(bericht);
    printf("Caesar versleuteld: %s\n", bericht);
}
// Deelfunctie die versleutelde caesar naar nederlands vertaalt
void caesar_ontsleutel(char *bericht) {
    for (int i = 0; bericht[i] != '\0'; i++) {
            char character = bericht[i];
            if (character >= 100) {
                bericht[i] = (char)(character - 3);
            } else {
                bericht[i] = (char)(character + 23);
            }
    }
}
// Vertaalt casar naar nederlands
void opdracht_caesar_ontsleutel(const char *fn) {
    char bericht[MAX_GROOTTE];

    lees_bestand(fn, bericht);
    versimpel(bericht);
    caesar_ontsleutel(bericht);
    printf("Caesar ontsleuteld: %s\n", bericht);
}
// Deelfunctie die nederlandse tekst versleuteld naar vigenere
void vigenere_versleutel(char *bericht, const char *sleutel) {
    char lange_sleutel[MAX_GROOTTE];

    size_t bericht_grootte = strlen(bericht);
    size_t sleutel_grootte = strlen(sleutel);

    size_t factor = bericht_grootte / sleutel_grootte;
    size_t rest = bericht_grootte % sleutel_grootte;

    for (size_t i = 0; i < factor; i++) {
        strncpy(lange_sleutel + (i * sleutel_grootte), sleutel, sleutel_grootte);
    }
    strncpy(lange_sleutel + (factor * sleutel_grootte), sleutel, rest);

    for (int i = 0; bericht[i] != '\0'; i++) {

        char sleutel_character = lange_sleutel[i];
        char character = bericht [i];

        int offset = (int)sleutel_character - 97;

        if (character + offset > 122) {
            bericht[i] = (char)(character + offset - 26);
        } else {
            bericht[i] = (char)(character + offset);
        }
    }
}
// Versleuteld tekst naar vignere
void opdracht_vigenere_versleutel(const char *fn, const char *sleutel) {
    char bericht[MAX_GROOTTE];

    lees_bestand(fn, bericht);
    versimpel(bericht);
    vigenere_versleutel(bericht, sleutel);
    printf("Vigenere versleuteld: %s\n", bericht);
}
// Deelfunctie die vigenere ontsleuteld naar nederlands
void vigenere_ontsleutel(char *bericht, const char *sleutel) {
    char lange_sleutel[MAX_GROOTTE];

    size_t bericht_grootte = strlen(bericht);
    size_t sleutel_grootte = strlen(sleutel);

    size_t factor = bericht_grootte / sleutel_grootte;
    size_t rest = bericht_grootte % sleutel_grootte;

    for (size_t i = 0; i < factor; i++) {
        strncpy(lange_sleutel + (i * sleutel_grootte), sleutel, sleutel_grootte);
    }
    strncpy(lange_sleutel + (factor * sleutel_grootte), sleutel, rest);

    for (int i = 0; bericht[i] != '\0'; i++) {
        char sleutel_character = lange_sleutel[i];
        char character = bericht [i];

        int offset = (int)sleutel_character - 97;

        if (character - offset < 97) {
            bericht[i] = (char)(character - offset + 26);
        } else {
            bericht[i] = (char)(character - offset);
        }
    }
}
// Ontsleuteld vigenere naar nederlands
void opdracht_vigenere_ontsleutel(const char *fn, const char *sleutel) {
    char bericht[MAX_GROOTTE];

    lees_bestand(fn, bericht);
    versimpel(bericht);
    vigenere_ontsleutel(bericht, sleutel);
    printf("Vigenere ontsleuteld: %s\n", bericht);
}
// De deelfunctie die de frequentie van letters in tekst bepaalt
void bepaal_frequenties(const char *tekst, double *p) {
    int letters = 0;
    for (int i = 0; tekst[i] != '\0'; i++) {
        char character = tekst[i];
            int offset = (int)character - 97;
            p[offset]++;
            letters++;
    }

    if (letters > 0) {
        for (int i = 0; i < 26; i++) {
            p[i] = p[i] / letters;
        }
    }
}
// Bepaalt de frequentie van letters in tekst
void opdracht_frequenties(const char *fn) {
    char bericht[MAX_GROOTTE];
    double frequenties[26];

    lees_bestand(fn, bericht);
    versimpel(bericht);
    bepaal_frequenties(bericht, frequenties);

    for (int i = 0; i < 26; i++) {
        printf("%c: %.3f\n", 97 + i, frequenties[i]);
    }
}
// Berekent het inproduct tussen 2 letter frequentie arrays
double gelijkenis(const double *p, const double *q) {
    double tot_inproduct = 0.0;

    for (int i = 0; i < 26; i++) {
        tot_inproduct += p[i] * q[i];
    }
    return tot_inproduct;
}
// Vergelijkt de letter frequentie arrays van twee teksten met elkaar d.m.v hun inproduct
void opdracht_vergelijk(const char fn_tekst1[], const char fn_tekst2[]) {
    char tekst[MAX_GROOTTE];

    double p[26];
    lees_bestand(fn_tekst1, tekst);
    versimpel(tekst);
    bepaal_frequenties(tekst, p);

    double q[26];
    lees_bestand(fn_tekst2, tekst);
    versimpel(tekst);
    bepaal_frequenties(tekst, q);

    printf("Gelijkenis is %.3f\n", gelijkenis(p, q));
}
//  Deelfunctie die de best passende sleutel zoekt die een gegeven vigenere cijfertekst kan vertalen naar nederlands
double zoek_sleutel(const char *cijfertekst, char *sleutel, size_t slen, const double *frequenties_taal) {
    char ontsleutelde_cijfertekst[MAX_GROOTTE];
    char prot_sleutel[17];
    double max_gelijkenis = 0.0;

    prot_sleutel[slen] = '\0';

    for (size_t i = 0; i < slen; i++) {
        prot_sleutel[i] = 97;
    }

    for (size_t i = 0; i < slen; i++) {
        for (char k = 97; k <= 122; k++) {
            prot_sleutel[i] = k;

            strcpy(ontsleutelde_cijfertekst, cijfertekst);
            vigenere_ontsleutel(ontsleutelde_cijfertekst, prot_sleutel);

            double frequenties_cijfertekst[26] = {0.0};
            bepaal_frequenties(ontsleutelde_cijfertekst, frequenties_cijfertekst);

            double gelijkenis_value = gelijkenis(frequenties_cijfertekst, frequenties_taal);

            if (gelijkenis_value > max_gelijkenis) {
                strcpy(sleutel, prot_sleutel);
                max_gelijkenis = gelijkenis_value;
            }
        }
    }
    return max_gelijkenis;
}
// Kraakt de sleutel van een gegeven cijferreeks
void opdracht_kraak(const char *fn_cijfertekst, const char *fn_taal) {
    char cijfertekst[MAX_GROOTTE];
    char taal[MAX_GROOTTE];
    char sleutel[17];
    double frequenties_taal[26];

    lees_bestand(fn_taal, taal);
    versimpel(taal);
    bepaal_frequenties(taal, frequenties_taal);

    lees_bestand(fn_cijfertekst, cijfertekst);
    versimpel(cijfertekst);

    for (size_t sleutel_lengte = 1; sleutel_lengte <= MAX_SLEUTEL; sleutel_lengte++) {
        double max_gelijkenis = zoek_sleutel(cijfertekst, sleutel, sleutel_lengte, frequenties_taal);
        printf("Sleutel %s: score= %.3f\n", sleutel, max_gelijkenis);
    }
}

void gebruik(void) {
    fprintf(stderr, "gebruik: vigenere caesar_versleutel <bericht>\n"
                    "         vigenere caesar_ontsleutel <bericht>\n"
                    "         vigenere versleutel <bericht> <sleutel>\n"
                    "         vigenere ontsleutel <cijfertekst> <sleutel>\n"
                    "         vigenere frequenties <tekst>\n"
                    "         vigenere vergelijk <tekst1> <tekst2>\n"
                    "         vigenere kraak <cijfertekst> <voorbeeld>\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

    if (argc < 2 || argc > 4) {
        gebruik();
    }

    const char *opdr = argv[1];
    if (strcmp(opdr, "caesar_versleutel")==0 && argc==3) {
        opdracht_caesar_versleutel(argv[2]);
    } else if (strcmp(opdr, "caesar_ontsleutel")==0 && argc==3) {
        opdracht_caesar_ontsleutel(argv[2]);
    } else if (strcmp(opdr, "versleutel")==0 && argc==4) {
        opdracht_vigenere_versleutel(argv[2], argv[3]);
    } else if (strcmp(opdr, "ontsleutel")==0 && argc==4) {
        opdracht_vigenere_ontsleutel(argv[2], argv[3]);
    } else if (strcmp(opdr, "frequenties")==0 && argc==3) {
        opdracht_frequenties(argv[2]);
    } else if (strcmp(opdr, "vergelijk")==0 && argc==4) {
        opdracht_vergelijk(argv[2], argv[3]);
    } else if (strcmp(opdr, "kraak")==0 && argc==4) {
        opdracht_kraak(argv[2], argv[3]);
    } else {
        gebruik();
    }
    return EXIT_SUCCESS;
}
