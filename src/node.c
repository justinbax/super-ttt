#include "node.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void initGame(Game *g, int depth) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            g->board[i][j] = EMPTY;
        }
        g->metaBoard[i] = EMPTY;
    }

    g->nextPl = P_X;
    g->depth = depth;
    g->mustMoveIn = ANYWHERE;

    g->nextMoveMetaPtr = ANYWHERE;
    g->nextMovePtr = 0;
}

Game *getNextMove(Game *g) {
    int i = g->nextMoveMetaPtr;
    if (i < 0) {
        // ANYWHERE or FINISHED_SEARCH
        if (i == FINISHED_SEARCH) {
            return NULL;
        }
        i = (g->mustMoveIn == ANYWHERE ? 0 : g->mustMoveIn);
    }

    for (; i < 9; i++) {
        // Loops for every i from 0 to 8 if mustMoveIn == ANYWHERE, loops once with i = mustMoveIn otherwise (breaks at the end)
        for (; g->nextMovePtr < 9; g->nextMovePtr++) {
            if (g->board[i][g->nextMovePtr] == EMPTY) {
                Game *new = malloc(sizeof(Game));
                initGame(new, g->depth - 1);
                memcpy(new->board, g->board, sizeof(uint8_t) * 9 * 9);
                memcpy(new->metaBoard, g->metaBoard, sizeof(uint8_t) * 9);
                new->board[i][g->nextMovePtr] = g->nextPl;
                new->nextPl = (g->nextPl == P_X ? P_O : P_X);

                if (new->metaBoard[i] == EMPTY) {
                    new->metaBoard[i] = generalGameIsFinished(new->board[i]);
                    new->mustMoveIn = (new->metaBoard[i] != NO_RESULT ? ANYWHERE : g->nextMovePtr);
                }

                g->nextMoveMetaPtr = i;
                g->nextMovePtr++;
                
                if (g->nextMovePtr > 8) {
                    g->nextMovePtr = 0;
                    g->nextMoveMetaPtr++;
                    if (g->nextMoveMetaPtr > 8 || g->mustMoveIn != ANYWHERE) {
                        g->nextMoveMetaPtr = FINISHED_SEARCH;
                    }
                }

                return new;
            }
        }
        if (g->mustMoveIn != ANYWHERE) {
            break;
        }
    }

    g->nextMoveMetaPtr = FINISHED_SEARCH;
    return NULL;
}

Node *getAllMoves(Game *g) {
    Node *prev = NULL;
    Game *move;
    while (1) {
        move = getNextMove(g);
        if (move == NULL) {
            break;
        }

        Node *new = malloc(sizeof(Node));
        initNode(new, prev, move);
        prev = new;
    }
    return prev;
}

float gameEvaluation(Game *g) {
    // TODO improve this heuristic
    int count = 0;
    for (int i = 0; i < 9; i++) {
        count += g->metaBoard[i] == P_X;
        count -= g->metaBoard[i] == P_O;
    }

    return count / 9.0f;
}

bool isTerminal(Game *g) {
    return generalGameIsFinished(g->metaBoard) != NO_RESULT;
}

int generalGameIsFinished(uint8_t *g) {
    for (int i = 0; i < 3; i++) {
        if ((((g[3*i] == g[3*i+1]) && (g[3*i] == g[3*i+2])) // Horizontal
         || ((g[i] == g[i+3]) && (g[i] == g[i+6]))) // Vertical
         && g[4*i] != EMPTY) { // Non-empty test on the only common square of both tests (square 0, 4 or 8)
            return g[4*i];
        }
    }

    // Diagonal
    if (((g[0] == g[4]) && (g[0] == g[8]))
     || ((g[2] == g[4]) && (g[2] == g[6]))) {
        return g[4];
     }

    return NO_RESULT;
}

void initNode(Node *n, Node *next, Game *g) {
    n->g = g;
    n->next = next;
}