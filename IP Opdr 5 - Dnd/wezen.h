/* Name: K. Klaassen
   UvAnetID: 12345678
   Studie: BSc Informatica

   Functiedefinities voor het type "wezen"; een entiteit in een Dungeons
   and Dragons spel.
   
   Het type "wezen" wordt hier gedeclareerd maar nog niet volledig
   gedefinieerd: het is een "incomplete type". Dat betekent dat als je
   deze header #include, je alleen *pointers* naar wezens kunt gebruiken.

   Daarom werken alle functie die hier worden gedeclareerd ook met pointers
   naar wezens.

   De daadwerkelijke definitie van het type gebeurt in "wezen.c".
*/

struct wezen;
typedef struct wezen wezen;

/* Werpt het gegeven aantal dobbelstenen van het aangegeven soort.

   aantal_dobbels: het aantal dobbelstenen dat wordt geworpen
   maximum: het hoogste getal op de dobbelsteen

   Bijvoorbeeld, worp(2,8) rolt 2d8.

   Uitvoer: de som van de worpen.
*/
int worp(int aantal_dobbels, int maximum);


/* Geeft het aantal deelnemers dat is geimplementeerd. 
*/
int aantal_deelnemers(void);


/* Initialiseert een van de deelnemers en alloceert er 
   ruimte voor op de heap.
   
   n: een getal in { 0, 1, ..., aantal_deelnemers()-1 }
   
   Uit: een pointer naar de n-de deelnemer.
*/
wezen *init_deelnemer(int n);


/* Geeft de naam van het wezen in string formaat.

   w: een pointer naar de het wezen.

   Uit: de naam.
*/
const char *naam(const wezen *w);


/* Geeft de naam van de soort van een wezen in string formaat.

   w: een pointer naar de het wezen.

   Uit: een string beschrijving.
*/
const char *soort_naam(const wezen *w);


/* Geeft de naam van de klasse van een wezen in string formaat.

   w: een pointer naar de het wezen.

   Uit: een string beschrijving.
*/
const char *klasse_naam(const wezen *w);


/* Berekent de strength bonus van dit wezen.

   Uit: (strength - 10) / 2, omlaag afgerond.
 */
int str_bonus(const wezen *w);


/* Berekent de dexterity bonus van dit wezen.

   Uit: (dexterity - 10) / 2, omlaag afgerond.
 */
int dex_bonus(const wezen *w);


/* Berekent de armor class van het wezen op basis van zijn
   armor en dexterity.

   w: een pointer naar het wezen.

   Uit: de AC.
*/
int armor_class(const wezen *w);


/* Geeft de huidige hoeveelheid hitpoints van het wezen terug.

   w: een pointer naar het wezen.

   Uit: het aantal HP.
*/
int hitpoints(const wezen *w);


/* Verandert de huidige hoeveelheid hitpoints van het wezen met
   het opgegeven aantal. Als het aantal onder 0 zou komen, wordt het 
   aantal hitpoints op 0 gesteld.

   w:        een pointer naar het wezen.
   delta_hp: de verandering in het aantal hitpoints.
*/
void wijzig_hitpoints(wezen *w, int delta_hp);


/* De hoeveelheid XP die dit wezen waard is als je het verslaat. 

   w: een pointer naar het wezen.

   Uit: de XP value.
*/
int xp_value(const wezen *w);


/* Verandert de hoeveelheid XP die dit wezen tot nu toe heeft verdiend.
   
   w: een pointer naar het wezen.
   delta_xp: de hoeveelheid XP die het wezen verdient.
*/
void wijzig_xp(wezen *w, int delta_xp);


/* Kijkt of een wezen een "player character" is.

   w: een pointer naar het wezen.

   Uit: 1 als het wezen een speler is, en anders 0.
*/
int is_pc(const wezen *w);



/* Beschrijft kort een wezen.

   w: een pointer naar het wezen.

   Side effect: drukt een beschrijving af naar stdout.
*/
void beschrijf(const wezen *w);


/* Bepaalt de attack bonus van een wezen. 

   w: een pointer naar het wezen.

   Uit: de attack bonus.
*/
int attack_bonus(const wezen *w);


/* Rolt de schade die het wapen doet. Bevat nog geen ability bonus
   en houdt geen rekening met critical hits.

   w:    een pointer naar het wezen.
   
   Uit: de hoeveelheid schade.
*/
int wapen_schade(const wezen *w);
