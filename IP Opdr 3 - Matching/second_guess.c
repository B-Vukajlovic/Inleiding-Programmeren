#include "rps.h"

#define MAX_RONDES 10000

/* Algo's:
   1. History match - my play beats his predicted play
   2. Second guessing history match - my play equals naive prediction
      (I naively predict he'll play stone, so he will think I say paper,
      so he'll actually play scissors, so I'll actually play stone)
   3. Third guessing history match - my play would lose to the preciction

    Time scales:
    - follow which was best with decay with mean 8,  decay is 1-1/8
    - follow which was best with decay with mean 16, decay is 1-1/16
    - follow which was best with decay with mean 32, decay is 1-1/32
    - follow which was best with decay with mean 64, decay is 1-1/64

    Each time scale favours an algorithm for current play.
    Each time scale incurs a score.
    We pick the best time play and follow it.
*/


int main(void) {

    keuze ik[MAX_RONDES], ander[MAX_RONDES];
    int match_lengte[MAX_RONDES];

    // double decays[] = { 1-1.0/8, 1-1.0/16, 1-1.0/32, 1-1.0/64 };
    // const int ndecays = sizeof(decays)/sizeof(double);
    // double decay_scores[] = { 0, 0, 0, 0 };
    
    // double scores[][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }};
    
    int scores[] = { 0, 0, 0 }; // regular, second guess, third guess


    rps_init();

    for (int ronde = 0;; ronde++) {
        keuze pred = ROCK;
        if (ronde > 0) {
            // update matches met de laatst gemaakte keuzes en onthoud de langste
            int langste_match_index = -1;
            for (int i=0; i<ronde-1; i++) {
                if (ik[i] == ik[ronde-1] && ander[i] == ander[ronde-1]) {
                    match_lengte[i]++;
                    if (match_lengte[i] >= langste_match_index) {
                        langste_match_index = i;
                    }
                } else {
                    match_lengte[i] = 0;
                }
            }
            match_lengte[ronde-1] = ronde-1;
            pred = wint_van(ik[langste_match_index+1]);
        }

        keuze keuzes[] = { wint_van(pred), pred, verliest_van(pred) };

        int strat = 0;
        if (scores[1] > scores[strat]) { strat = 1; }
        if (scores[2] > scores[strat]) { strat = 2; }
        keuze k = keuzes[strat];

        spreek(k);
        keuze r = luister();
        if (r == STOP) { 
            break;
        }
        for (int i=0; i<3; i++) {
            if (keuzes[i] == wint_van(r)) { 
                scores[i] += 2; 
            } else if (keuzes[i] == r) {
                scores[i] += 1; 
            }
        }

        ik[ronde] = k;
        ander[ronde] = r;
    }

    return 0;
}

