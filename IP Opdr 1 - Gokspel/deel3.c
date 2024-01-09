#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float gok;
float max = 100;
float min = 0;
int niet_gegokt = 0;
float middle;
int code;

int main(void) {

    while(niet_gegokt == 0) {
        
        printf("Doe een gok:\n");
        code = scanf("%f", &gok);
        middle = ((max + min)/2);

        if (gok < 0 || gok > 100 || code != 1) {
            printf("Ik begrijp de invoer niet.\n\n");
            return 0;
        }

        if ((gok == min + 1) && (gok == max - 1)) {
            niet_gegokt = 1;
            printf("Je hebt het geraden!\n\n");
        } else if ((gok <= max) && ((gok - middle) >= 0)) {
            max = gok;
            printf("Te hoog.\n\n"); 
        } else if ((gok >= min) && ((gok - middle) < 0)) {
            min = gok;
            printf("Te laag.\n\n");
        } else if (gok > max) {
            printf("Te hoog.\n\n"); 
        } else if (gok < min) {
            printf("Te laag.\n\n"); 
        }   
    } 
return 0;
}   


