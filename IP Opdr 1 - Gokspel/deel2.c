#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(void) {
    for(int i = 1; i<= 13; i++) {
        for (int j = 1; j <= (i* 2) - 2; j++) {
            printf(" ");
        }   
        printf("*");
        
        for (int j = 1; j <= (i* 2) - 2; j++) {
            printf(".");
        }
        printf("*");
        printf("\n");
    }

    for(int i = 14; i<= 23; i++) {
        for (int j = 1; j <= (i* 2) - 2; j++) {
            printf(" ");
        }
        printf("*");
        
        for (int j = 1; j <= ((i * 2) - 2) - (5 * (i-14)); j++) {
            printf(".");
        }

        if (i == 23) {
        } else {
            printf("*");
        }

        printf("\n");
    
    }
return 0;
}