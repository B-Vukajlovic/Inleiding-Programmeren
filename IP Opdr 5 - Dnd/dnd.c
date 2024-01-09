/* Name: K. Klaassen
   UvAnetID: 12345678
   Studie: BSc Informatica
  
   Dit programma implementeert een deel van de regels van D&D5e, en simuleert
   een eenvoudig gevecht.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "wezen.h"

/* Stel de speler in staat om een monster te kiezen om aan te vallen.

   deelnemers:   een array met alle deelnemers aan deze encounter.
   n_deelnemers: het aantal deelnemers in de array.

   Uit: een pointer naar het gekozen wezen.
*/
wezen *pc_kiest_doelwit(wezen **deelnemers, int n_deelnemers) {

    printf("Wie val je aan?\n");
    
    // 1. Beschrijf mogelijke keuzes.
    int n_keuzes = 0;
    for (int i=0; i < n_deelnemers; i++) {
        wezen *w = deelnemers[i];
        if (!is_pc(w) && hitpoints(w) > 0) {
            printf("  %d. ", n_keuzes+1);
            beschrijf(w);
            n_keuzes++;
        }
    }

    // 2. Maak keuze.
    int keuze;
    while (1) {
        printf("Maak je keuze: >");
        fflush(stdout);
        int ingevoerd = scanf("%d", &keuze);
        if (ingevoerd == 1  && keuze >= 1 && keuze <= n_keuzes) {
            break;
        }
        printf("Keuze niet begrepen, probeer nog eens?\n");
    }

    // 3. Geef het gekozen wezen terug.
    wezen *w = NULL;
    for (int i = 0; keuze > 0; i++) {
        w = deelnemers[i];
        if (!is_pc(w) && hitpoints(w) > 0) {
            keuze--;
        }
    }

    return w;
}


/* Kies een willekeurig levend persoon om aan te vallen.

   deelnemers:   een array met alle deelnemers aan deze encounter.
   n_deelnemers: het aantal deelnemers in de array.

   Uit: een willekeurig gekozen levend persoon.
*/
wezen *npc_kiest_doelwit(wezen **deelnemers, int n_deelnemers) {
    /* Ik doe dit met Reservoir Sampling. Zonder deze truc moet
       je eerst tellen hoeveel geschikte deelnemers er zijn.
    */
    wezen *w_keuze = NULL;
    int k = 1;
    for (int i = 0; i < n_deelnemers; i++) {
        wezen *w = deelnemers[i];
        if (is_pc(w) && hitpoints(w) > 0 && worp(1, k) == 1) {
            w_keuze = w;
            k++;
        }
    }
    return w_keuze;
}


/* Simulateert dat een wezen een ander aanvalt. De wezens worden hier
   niet geupdated, maar de functie geeft het aantal hitpoints terug dat de
   verdediger verliest (of 0 als de aanval miste).

   aanvaller:  een pointer naar het aanvallende wezen.
   verdediger: een pointer naar het verdedigende wezen.

   Uit: de hoeveelheid schade die de aanval toebrengt.

   Side effect: de functie geeft ook informatie aan de speler over de aanval.
*/
int aanval(const wezen *aanvaller, const wezen *verdediger) {
    printf("%s valt %s aan. %s heeft %d nog hp.\n", 
        naam(aanvaller),
        naam(verdediger), 
        naam(verdediger),
        hitpoints(verdediger));
    
    int attack_roll = worp(1, 20);
    if (attack_roll == 1) { 
        // 1. Critical miss
        printf("- %s rolt 1. Critical miss! De aanval mislukt smadelijk.\n",
            naam(aanvaller));
        return 0;
    }
    
    int att_b = attack_bonus(aanvaller);
    int str_b = str_bonus(aanvaller);

    if (attack_roll == 20) {
        // 2. Critical hit
        int schade = wapen_schade(aanvaller) + wapen_schade(aanvaller);
        printf("- %s rolt 20. Critical hit! de aanval "
            "doet %d + %d = %d schade.\n",
            naam(aanvaller), schade, str_b, schade + str_b);
        return schade + str_b;
    }

    int ac = armor_class(verdediger);
    printf("- %s rolt %d + %d = %d. De AC van %s is %d. ",
        naam(aanvaller), attack_roll, att_b,
        attack_roll + att_b, naam(verdediger), ac);

    if (attack_roll + att_b < ac) {
        // 3. Gemist
        printf("De aanval mislukt.\n");
        return 0;
    }

    // 4. Geraakt
    printf("De aanval raakt!\n");
    int schade = wapen_schade(aanvaller);
    printf("- De aanval doet %d + %d = %d schade.\n", 
        schade, str_b, schade + str_b);
    
    return schade + str_b;
}


/* Introduceer de speler tot het spel en beschrijf de wezens.

   deelnemers:   een array met alle deelnemers aan deze encounter.
   n_deelnemers: het aantal deelnemers in de array.
*/
void introductie(wezen **deelnemers, int n_deelnemers) {
    printf("Het is een mooie lentedag, en je bent op avontuur met je\n");
    printf("vertrouwde groep metgezellen:\n");
    for (int i = 0; i < n_deelnemers; i++) {
        if (is_pc(deelnemers[i])) { 
            printf(" - ");
            beschrijf(deelnemers[i]);
        }
    }
    printf("\n");
    printf("Je dag verloopt voorspoedig, tot je plots wordt aangevallen!\n");
    printf("Laten we hopen dat de vrienden deze ontmoeting overleven!\n\n");
}


/* Simuleert een volledige encounter, totdat ofwel alle personen dood zijn,
   of alle monsters.

   deelnemers:   een array met alle deelnemers aan deze encounter.
   n_deelnemers: het aantal deelnemers in de array.
*/
void encounter(wezen **deelnemers, int n_deelnemers) {
    
    int n_vrienden_in_leven = 0;
    int n_monsters_in_leven = 0;
    for (int i = 0; i < n_deelnemers; i++) {
        if (hitpoints(deelnemers[i]) == 0) {
            continue;
        }
        if (is_pc(deelnemers[i])) {
            n_vrienden_in_leven++;
        } else {
            n_monsters_in_leven++;
        }
    }

    for (int beurt = 0;
         n_vrienden_in_leven > 0 && n_monsters_in_leven > 0;
         beurt = (beurt + 1) % n_deelnemers) {
        if (hitpoints(deelnemers[beurt]) == 0) {
            continue; // Dit wezen is dood; sla beurt over.
        }

        printf("De beurt is aan %s.\n", naam(deelnemers[beurt]));

        wezen *verdediger;
        if (is_pc(deelnemers[beurt])) {
            verdediger = pc_kiest_doelwit(deelnemers, n_deelnemers);
        } else {
            verdediger = npc_kiest_doelwit(deelnemers, n_deelnemers);
        }
        const wezen *aanvaller = deelnemers[beurt];
        int dmg = aanval(aanvaller, verdediger);
        wijzig_hitpoints(verdediger, -dmg);
        if (hitpoints(verdediger) == 0) {   
            if (is_pc(verdediger)) {
                n_vrienden_in_leven--;
                printf("O nee! %s de %s heeft %s gedood! :'(\n",
                    naam(aanvaller), soort_naam(aanvaller), naam(verdediger));
            } else {
                n_monsters_in_leven--;
                int xp_gain = xp_value(verdediger);
                printf("%s heeft %s de %s gedood! De groep verdient %d xp.\n",
                    naam(aanvaller), naam(verdediger),
                    soort_naam(verdediger), xp_gain);
                for (int i = 0; i < n_deelnemers; i++) {
                    if (is_pc(deelnemers[i]) && hitpoints(deelnemers[i]) > 0) {
                        wijzig_xp(deelnemers[i], xp_gain / n_vrienden_in_leven);
                    }
                }
            }
        }
        printf("\n");
    }
}


/* Beschrijft de afloop van de strijd.

   deelnemers:   een array met alle deelnemers aan deze encounter.
   n_deelnemers: het aantal deelnemers in de array.
*/
void conclusie(wezen **deelnemers, int n_deelnemers) {
    int n_vrienden_in_leven = 0;
    int n_vrienden_dood = 0;
    for (int i = 0; i < n_deelnemers; i++) {
        if (is_pc(deelnemers[i])) {
            if (hitpoints(deelnemers[i]) > 0) { 
                n_vrienden_in_leven++;
            } else {
                n_vrienden_dood++;
            }
        }
    }

    if (n_vrienden_in_leven == 0) {
        printf("Het kwaad zegeviert op deze donkere dag. De dappere\n");
        printf("avonturiers zijn allemaal dood, en de monsters knagen op hun\n");
        printf("botten.\n");
    } else if (n_vrienden_dood > 0) {
        printf("Je hebt de afschuwelijke monsters verslagen, maar alles\n");
        printf("heeft een prijs. Je hebt %d ", n_vrienden_dood);
        printf("van je dappere kameraden verloren.\n");
        printf("Hun offers zullen niet worden vergeten.\n");
    } else {
        printf("Je hebt gewonnen! De metgezellen zijn erin geslaagd om elk\n");
        printf("monster te verlaan. De heroische strijd van vandaag zal\n");
        printf("nog lang worden bezongen.\n");
    }
}


int main(void) {
    
    srand((unsigned)time(NULL));

    int n_deelnemers = aantal_deelnemers();
    wezen *deelnemers[n_deelnemers];
    for (int i = 0; i < n_deelnemers; i++) {
        deelnemers[i] = init_deelnemer(i);
    }
    
    introductie(deelnemers, n_deelnemers);
    encounter(deelnemers, n_deelnemers);
    conclusie(deelnemers, n_deelnemers);

    for (int i = 0; i < n_deelnemers; i++) {
        free(deelnemers[i]);
    }

    return 0;
}
