#include "node.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void initGame(Game *g, int depth) {
    memset(g->board, EMPTY, 81);
    memset(g->metaBoard, NO_RESULT, 9);
    memset(g->boardOccupations, 0, 9);

    g->nextPl = P_X;
    g->depth = depth;
    g->mustMoveIn = ANYWHERE;
    g->score = 0;

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
        if (g->metaBoard[i] != NO_RESULT) {
            continue;
        }
        for (; g->nextMovePtr < 9; g->nextMovePtr++) {

            if (g->board[i][g->nextMovePtr] == EMPTY) {
                Game *new = malloc(sizeof(Game));
                initGame(new, g->depth - 1);
                memcpy(new->board, g->board, sizeof(uint8_t) * 9 * 9);
                memcpy(new->metaBoard, g->metaBoard, sizeof(uint8_t) * 9);
                memcpy(new->boardOccupations, g->boardOccupations, sizeof(uint8_t) * 9);
                new->board[i][g->nextMovePtr] = g->nextPl;
                new->boardOccupations[i]++;
                new->nextPl = -1 * g->nextPl;

                int subgameStatus = generalGameIsFinishedFromMove(new->board[i], g->nextMovePtr, g->boardOccupations[i]);
                new->metaBoard[i] = subgameStatus;
                if (subgameStatus == P_X) {
                    new->score++;
                } else if (subgameStatus == P_O) {
                    new->score--;
                }

                new->mustMoveIn = (new->metaBoard[i] != NO_RESULT ? ANYWHERE : g->nextMovePtr);

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
    return g->score / 9.0f;
}

int generalGameIsFinishedFromMove(uint8_t *g, int pos, int filled) {
    if (filled == 9) {
        return TIE;
    }

    if (g[4] != EMPTY) {
        // Both diags
        if (g[0] == g[4] && g[4] == g[8]) {
            return g[4];
        } else if (g[2] == g[4] && g[4] == g[6]) {
            return g[4];
        }
        // Vertical
        else if (g[1] == g[4] && g[4] == g[7]) {
            return g[4];
        }
        // Horizontal
        else if (g[3] == g[4] && g[4] == g[5]) {
            return g[4];
        }

    } else if (g[0] != EMPTY) {
        // Vertical
        if (g[0] == g[3] && g[3] == g[6]) {
            return g[0];
        }
        // Horizontal
        else if (g[0] == g[1] && g[1] == g[2]) {
            return g[0];
        }

    } else if (g[8] != EMPTY) {
        // Vertical
        if (g[2] == g[5] && g[5] == g[8]) {
            return g[8];
        }
        // Horizontal
        else if (g[6] == g[7] && g[7] == g[8]) {
            return g[8];
        }
    }

    return NO_RESULT;
}

void initNode(Node *n, Node *next, Game *g) {
    n->g = g;
    n->next = next;
}