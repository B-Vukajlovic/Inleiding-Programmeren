#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int x;
int y;
int a;
int b;
int code;

int main(void) {
    
    printf("Geef de waarde voor a:\n");
    code = scanf("%d", &a); 
    
    if (code != 1) {
        printf("Het lezen van een getal is niet goed gelukt. Ik stop ermee.\n");
        return 1;
    }; 

    printf("Geef de waarde voor b:\n");
    code = scanf("%d", &b);
   
    if (code != 1) {
        printf("Het lezen van een getal is niet goed gelukt. Ik stop ermee.\n");
        return 0;
    };

    x = (a + b)/2 ;
    y = (a - b)/2 ;

    printf("x = %d\n", x);
    printf("y = %d\n", y);

return 0;
    
}