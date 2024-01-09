/* Name: B.Vukajlovic
   UvAnetID: 15225054
   Studie: BSc Informatica
  
   Dit programma speelt een verkorte versie van DND, totdat alle monsters of alle spelers verslagen zijn.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "wezen.h"

// Declaratie van de soort enum, bepaalt de de mogelijke soorten van een wezen
typedef enum {
    Dragonborn, Half_Orc, Dwarf, Hafling, Elf, Human, Gnome, Tiefling, Half_Elf
} soort;

// Declaratie van de klasse enum, bepaalt de mogelijke klassen van een wezen
typedef enum {
    Barbarian, Bard, Cleric, Druid, Fighter, Monk, Paladin, Ranger, Rogue, Sorcerer, Warlock, Wizard
} klasse;

// Declaratie van de proficiency enum, bepaalt de mogelijke proficiencies van een wezen
typedef enum {
    Sword, Axe, AWP, Spear, AK_47, SMG
} proficiency;

// Declaratie van de wapen struct, deze struct bevat de algemene structuur van een wapen
typedef struct {
    char wapen_naam[20];
    int dicecount;
    int dicemaximum;
    int attackbonus;
    int proficiencybonus;

} wapen;

// Declaratie van de armor struct, deze struct bevat de algemene structuur van armor
typedef struct {
    int armor_class;
} armor;

// Declaratie van de abilities struct, deze struct bevat de algemene structuur van de abilities die een wezen heeft
typedef struct {
    int DEX;
    int STR;
    int CON;
    int INT;
    int WIS;
    int CHA;
} abilities;

// Declaratie van de pc_stats struct, deze struct bevat de algemene structuur van de stats die een pc heeft
struct pc_stats {
    int level;
    int xp;
};
// Declaratie van de npc_stats struct, deze struct bevat de algemene structuur van die stats die een npc heeft
struct npc_stats {
    int xp_reward;
};

// Declaratie van de combi union, deze union bepaalt of een wezen npc of pc stats heeft
union combi {
    struct npc_stats npc_stats;
    struct pc_stats pc_stats;
};

// Declaratie van de wezen struct, deze struct bevat de algemene structuur van een wezen binnen dnd
struct wezen {
    int ispc;
    soort soort_wezen;
    klasse klasse_wezen;
    char naam[50];
    wapen wapen_wezen;
    int hp;
    proficiency proficiency_wezen;
    armor armor_wezen;
    abilities abilities_wezen;
    union combi stats;
};

/* Berekent de waarde van een dobbelsteen worp
Invoer:
    int aantal_dobbels = Het aantal dobbels die gegooid worden
    int maximum = De maximale grote van een dobbelsteen
Uitvoer:
    int som = De waarde van een worp
*/
int worp(int aantal_dobbels, int maximum) {
    int som = 0;
    for (int i = 0; i < aantal_dobbels; i++) {
        som += 1 + (rand()%maximum);
    }
    return som;
}

/* Initialiseert speler 1 met het type struct wezen
Invoer:
    void
Uitvoer:
    wezen speler_1 = De geinitisaliseerde speler 1 van het type struct wezen
*/
wezen init_speler1(void) {
    wezen speler_1 = {
        .ispc = 1,
        .naam = "Tito",
        .soort_wezen = Human,
        .klasse_wezen = Paladin,
        .wapen_wezen = {
            .wapen_naam = "AK_47",
            .dicecount = 3,
            .dicemaximum = 6,
            .proficiencybonus = 0
        },
        .hp = 17,
        .proficiency_wezen = Sword,
        .armor_wezen.armor_class = 12,
        .abilities_wezen = {
            .DEX = 14,
            .INT = 12,
            .STR = 12,
            .CON = 10,
            .WIS = 8,
            .CHA = 12
        },
        .stats = {
            .pc_stats = {
                .level = 6,
                .xp =  90
            }
        }
    };
    return speler_1;
}

/* Initialiseert speler 2 met het type struct wezen
Invoer:
    void
Uitvoer:
    wezen speler_2 = De geinitisaliseerde speler 2 van het type struct wezen
*/
wezen init_speler2(void) {
    wezen speler_2 = {
        .ispc = 1,
        .naam = "Stalin",
        .soort_wezen = Half_Orc,
        .klasse_wezen = Warlock,
        .wapen_wezen = {
            .wapen_naam = "AWP",
            .dicecount = 2,
            .dicemaximum = 20,
            .proficiencybonus = 2
        },
        .hp = 3,
        .proficiency_wezen = AWP,
        .armor_wezen.armor_class = 7,
        .abilities_wezen = {
            .DEX = 16,
            .INT = 8,
            .STR = 7,
            .CON = 16,
            .WIS = 14,
            .CHA = 16
        },
        .stats = {
            .pc_stats = {
                .level = 8,
                .xp =  80
            }
        }
    };
    return speler_2;
}

/* Initialiseert speler 3 met het type struct wezen
Invoer:
    void
Uitvoer:
    wezen speler_3 = De geinitisaliseerde speler 3 van het type struct wezen
*/
wezen init_speler3(void) {
    wezen speler_3 = {
        .ispc = 1,
        .naam = "Mussolini",
        .soort_wezen = Tiefling,
        .klasse_wezen = Druid,
        .wapen_wezen = {
            .wapen_naam = "Sword of Italy",
            .dicecount = 4,
            .dicemaximum = 5,
            .proficiencybonus = 2
        },
        .hp = 11,
        .proficiency_wezen = Sword,
        .armor_wezen.armor_class = 14,
        .abilities_wezen = {
            .DEX = 12,
            .INT = 8,
            .STR = 12,
            .CON = 9,
            .WIS = 14,
            .CHA = 16
        },
        .stats = {
            .pc_stats = {
                .level = 7,
                .xp =  70
            }
        }
    };
    return speler_3;
}

/* Initialiseert npc 1 met het type struct wezen
Invoer:
    void
Uitvoer:
    wezen monster_1 = De geinitisaliseerde npc 1 van het type struct wezen
*/
wezen init_monster1(void) {
    wezen monster_1 = {
        .ispc = 0,
        .naam = "Firing Squad",
        .soort_wezen = Hafling,
        .wapen_wezen = {
            .wapen_naam = "Musket",
            .dicecount = 6,
            .dicemaximum = 3,
            .proficiencybonus = 0
        },
        .hp = 14,
        .proficiency_wezen = AWP,
        .armor_wezen.armor_class = 12,
        .abilities_wezen = {
            .DEX = 11,
            .INT = 12,
            .STR = 14,
            .CON = 11,
            .WIS = 8,
            .CHA = 14
        },
        .stats = {
            .npc_stats = {
                .xp_reward = 15
            }
        }
    };
    return monster_1;
}

/* Initialiseert npc 2 met het type struct wezen
Invoer:
    void
Uitvoer:
    wezen monster_2 = De geinitisaliseerde npc 2 van het type struct wezen
*/
wezen init_monster2(void) {
    wezen monster_2 = {
        .ispc = 0,
        .naam = "Terrorist",
        .soort_wezen = Tiefling,
        .wapen_wezen = {
            .wapen_naam = "Ak-47",
            .dicecount = 2,
            .dicemaximum = 8,
            .proficiencybonus = 2
        },
        .hp = 15,
        .proficiency_wezen = AK_47,
        .armor_wezen.armor_class = 12,
        .abilities_wezen = {
            .DEX = 13,
            .INT = 10,
            .STR = 9,
            .CON = 8,
            .WIS = 8,
            .CHA = 13
        },
        .stats = {
            .npc_stats = {
                .xp_reward = 20
            }
        }
    };
    return monster_2;
}

/* Initialiseert npc 3 met het type struct wezen
Invoer:
    void
Uitvoer:
    wezen monster_3 = De geinitisaliseerde npc 3 van het type struct wezen
*/
wezen init_monster3(void) {
    wezen monster_3 = {
        .ispc = 0,
        .naam = "Sams Tailor",
        .soort_wezen = Dragonborn,
        .wapen_wezen = {
            .wapen_naam = "SMG",
            .dicecount = 5,
            .dicemaximum = 4,
            .proficiencybonus = 2
        },
        .hp = 13,
        .proficiency_wezen = SMG,
        .armor_wezen.armor_class = 11,
        .abilities_wezen = {
            .DEX = 9,
            .INT = 12,
            .STR = 14,
            .CON = 11,
            .WIS = 8,
            .CHA = 14
        },
        .stats = {
            .npc_stats = {
                .xp_reward = 25
            }
        }
    };
    return monster_3;
}

/*
Bepaalt het aantal deelnemers in het spel
Invoer:
    void
Uitvoer:
    int: Het aaantal deelnemers in het spel
*/
int aantal_deelnemers(void) {
    return 6;
}

/* Maakt een pointer die wijst naar de gewilde deelnemer op heap memory, hiervoor wordt eerst ruimte op de heap gereseveerd en wordt vervolgens
    de desbetreffende deelnemer gekopieerd naar de heap.
Invoer:
    int n: De desbetreffende deelnemer in het spel
Uitvoer:
    wezen *: Een pointer naar de desbetreffende deelnemer op heap memory
*/
wezen *init_deelnemer(int n) {
    wezen *w = malloc(sizeof(wezen));

    if (w == NULL) {
        perror("Help\n");
        exit(1);
    }

    if (n == 0) {
        *w = init_speler1();
        return w;
    } else if (n == 1) {
        *w = init_speler2();
        return w;
    } else if (n == 2) {
        *w = init_speler3();
        return w;
    } else if (n == 3) {
        *w = init_monster1();
        return w;
    } else if (n == 4) {
        *w = init_monster2();
        return w;
    } else if (n == 5) {
        *w = init_monster3();
        return w;
    } else {
        exit(1);
    }
}

/* Geeft de naam van het wezen in string formaat.
Invoer:
    wezen *w: Een pointer naar de het wezen.
Uitvoer:
    const char *: De naam van het wezen
*/
const char *naam(const wezen *w) {
    return w -> naam;
}

/* Geeft de naam van de soort van een wezen in string formaat.
Invoer:
    const wezen *w: Een pointer naar de het wezen.
Uitvoer:
    const char *: De soort van het wezen
*/
const char *soort_naam(const wezen *w) {
    switch (w->soort_wezen) {
        case Dragonborn: return "Dragonborn";
        case Half_Orc: return "Half_Orc";
        case Dwarf: return "Dwarf";
        case Hafling: return "Hafling";
        case Elf: return "Elf";
        case Human: return "Human";
        case Gnome: return "Gnome";
        case Tiefling: return "Tiefling";
        case Half_Elf: return "Half_Elf";
        default: return "unknown";
    }
}

/* Geeft de naam van de klasse van een wezen in string formaat.
Invoer:
   const wezen *w: Een pointer naar de het wezen.
Uitvoer:
   const char *: De klasse van het wezen
*/
const char *klasse_naam(const wezen *w) {
    if (is_pc(w) == 1) {
        switch (w->klasse_wezen) {
            case Barbarian: return "Barbarian";
            case Bard: return "Bard";
            case Cleric: return "Cleric";
            case Druid: return "Druid";
            case Fighter: return "Fighter";
            case Monk: return "Monk";
            case Paladin: return "Paladin";
            case Ranger: return "Ranger";
            case Rogue: return "Rogue";
            case Sorcerer: return "Sorcerer";
            case Warlock: return "Warlock";
            case Wizard: return "Wziard";
            default: return "unknown";
        }
    } else {
        return "unkown";
    }
}

/* Berekent de strength bonus van dit wezen.
Invoer:
    const wezen *w: Een pointer naar het wezen
Uitvoer:
    int: (strength - 10) / 2, omlaag afgerond.
 */
int str_bonus(const wezen *w) {
    int str = (w->abilities_wezen.STR);
    int str_bonus = (int)floor((str-10)/2);
    return str_bonus;
}

/* Berekent de dexterity bonus van dit wezen.
Invoer:
    const wezen *w: Een pointer naar het wezen
Uitvoer:
    int: (dexterity - 10) / 2, omlaag afgerond.
 */
int dex_bonus(const wezen *w) {
    int dex = (w->abilities_wezen.DEX);
    int dex_bonus = (int)floor((dex-10)/2);
    return dex_bonus;
}

/* Berekent de armor class van het wezen op basis van zijn
   armor en dexterity.
Invoer:
   wezen *w: Een pointer naar het wezen.
Uitvoer:
   int: De armor class van het wezen
*/
int armor_class(const wezen *w) {
    int armor_class = w->armor_wezen.armor_class + dex_bonus(w);
    return armor_class;
}

/* Geeft de huidige hoeveelheid hitpoints van het wezen terug.
Invoer:
   wezen *w: Een pointer naar het wezen.
Uitvoer:
   int: Het aantal HP.
*/
int hitpoints(const wezen *w) {
    return w->hp;
}

/* Verandert de huidige hoeveelheid hitpoints van het wezen met
   het opgegeven aantal. Als het aantal onder 0 zou komen, wordt het 
   aantal hitpoints op 0 gesteld.
Invoer:
    wezen *w: Een pointer naar het wezen.
    int delta_hp: De verandering in het aantal hitpoints.
Uitvoer:
    void
*/
void wijzig_hitpoints(wezen *w, int delta_hp) {
    w->hp += delta_hp;
    if (w->hp <= 0) {
        w->hp = 0;
    }
}

/* De hoeveelheid XP die dit wezen waard is als je het verslaat. 
Invoer:
   wezen *w: Een pointer naar het wezen.
Uitvoer:
   int: De XP value.
*/
int xp_value(const wezen *w) {
    if (is_pc(w) == 0) {
        return w->stats.npc_stats.xp_reward;
    } else {
        return 0;
    }
}

/* Verandert de hoeveelheid XP die dit wezen tot nu toe heeft verdient en zorgt voor een level up indien xp groter is dan 100.
Invoer:
    wezen *w: Een pointer naar het wezen.
    int delta_xp: De hoeveelheid XP die het wezen verdient.
Uitvoer:
    void
*/
void wijzig_xp(wezen *w, int delta_xp) {
    if (is_pc(w) == 1) {
        w->stats.pc_stats.xp = w->stats.pc_stats.xp + delta_xp;

        if (w->stats.pc_stats.xp >= 100) {
            w->stats.pc_stats.xp -= 100;
            w->stats.pc_stats.level++;
        }
    }
}

/* Kijkt of een wezen een "player character" is.
Invoer:
   wezen *w: Een pointer naar het wezen.
Uitvoer:
   int: 1 als het wezen een speler is, en anders 0.
*/
int is_pc(const wezen *w) {
    if(w->ispc == 1) {
        return 1;
    } else {
        return 0;
    }
}

/* Beschrijft kort een wezen.
Invoer:
    wezen *w: Een pointer naar het wezen.
Uitvoer:
    void
Side effect:
    Drukt een beschrijving af naar stdout.
*/
void beschrijf(const wezen *w) {
    if (is_pc(w) == 1) {
        printf("%s, een %s, en een level %d %s met %d HP en %d AC. Met als wapen een %s\n",
            naam(w), soort_naam(w), w->stats.pc_stats.level, klasse_naam(w), hitpoints(w), armor_class(w), w->wapen_wezen.wapen_naam);
    } else {
        printf("%s, een %s, met %d HP en %d AC. Met als wapen een %s\n",
            naam(w), soort_naam(w), hitpoints(w), armor_class(w), w->wapen_wezen.wapen_naam);
    }

}

/* Bepaalt de proficiency bonus van een wezen.
Invoer:
   wezen *w: Een pointer naar het wezen.
Uitvoer:
   int: De proficiency bonus
*/
int proficiency_bonus(const wezen *w) {
    if (w->wapen_wezen.proficiencybonus == 2) {
        return 2;
    } else {
        return 0;
    }
}

/* Bepaalt de attack bonus van een wezen. 
Invoer:
   wezen *w: Een pointer naar het wezen.
Uitvoer:
   int: De attack bonus.
*/
int attack_bonus(const wezen *w) {
    return proficiency_bonus(w) + str_bonus(w);
}

/* Berekent de schade die het wapen doet. 
Invoer:
   wezen *w: Een pointer naar het wezen.
Uitvoer:
   int : De hoeveelheid schade.
*/
int wapen_schade(const wezen *w) {
    return (worp(w->wapen_wezen.dicecount, w->wapen_wezen.dicemaximum) - 1) + attack_bonus(w);
}
