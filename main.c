#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "onthefly.h"

void reset_cache() {
    for (int i = 0; i < MAX_STATES; i++) {
        if (all_states[i] != NULL) {
            free(all_states[i]->out);
            free(all_states[i]);
            all_states[i] = NULL;
        }
    }
}

int main() {
    const char* f2 = "graphe.txt";
    const char* f3 = "graphe2.txt";

    printf("========== Tests de base sur mon_graphe2.txt ==========\n");
    printf("q0(p)->q1(empty)->q3(empty,leaf), q0(p)->q2(q)->q4(q,leaf)\n\n");

    printf("=== EF(q) ===\n");
    reset_cache();
    int r = check_EF(0, 'q', f2);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    printf("=== EF(p) ===\n");
    reset_cache();
    r = check_EF(0, 'p', f2);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    printf("=== EX(q) depuis q0 ===\n");
    reset_cache();
    r = check_EX(0, 'q', f2);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    printf("=== EX(p) depuis q0 ===\n");
    reset_cache();
    r = check_EX(0, 'p', f2);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    printf("=== EG(q) ===\n");
    reset_cache();
    r = check_EG(0, 'q', f2);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    printf("=== EG(p) ===\n");
    reset_cache();
    r = check_EG(0, 'p', f2);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    printf("========== Tests imbriques sur mon_graphe2.txt ==========\n\n");

    printf("=== EF(p ET EF(q)) ===\n");
    reset_cache();
    r = check_nested_EF_EF(0, 'p', 'q', f2);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    printf("=== EF(q ET EF(p)) ===\n");
    reset_cache();
    r = check_nested_EF_EF(0, 'q', 'p', f2);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    printf("========== Tests imbriques sur mon_graphe3.txt ==========\n");
    printf("q0(empty)->q1(p)->q3(q,leaf), q0(empty)->q2(empty)->q4(p,leaf)\n\n");

    printf("=== EF(p ET EF(q)) ===\n");
    reset_cache();
    r = check_nested_EF_EF(0, 'p', 'q', f3);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    printf("=== EF(p ET EF(z)) - z n'existe pas ===\n");
    reset_cache();
    r = check_nested_EF_EF(0, 'p', 'z', f3);
    printf("Resultat : %s\n\n", r ? "VRAI" : "FAUX");

    return 0;
}