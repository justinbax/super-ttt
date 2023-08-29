#include <stdio.h>

#include "node.h"

float minimax(Game *g, bool isX) {

    //printf("Depth %i\n", g->depth);

    if (g->depth == 0) {
        return gameEvaluation(g);
    }

    Node *moves = possibleMoves(g);
    if (moves == NULL) {
        printf("No available moves found.\n");
        return gameEvaluation(g);
    }

    float best = (isX ? -10.0f : 10.0f);
    while (moves != NULL) {
        
        float eval = minimax(moves->g, !isX);
        if (isX) {
            best = (eval > best ? eval : best);
        } else {
            best = (eval < best ? eval : best);
        }


        Node *old = moves;
        moves = moves->next;
        free(old->g);
        free(old);
    }

    return best;
}

int main() {
    printf("super-ttt\n");

    Game initial;
    initGame(&initial, 8);

    float e = minimax(&initial, true);
    printf("%f\n", e);

    return 0;
}