#include "node.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initGame(Game *g, int depth) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            g->board[i][j] = EMPTY;
        }
        g->metaBoard[i] = EMPTY;
    }

    g->next = X;
    g->depth = depth;
    g->mustMoveIn = ANYWHERE;
}

Node *possibleMoves(Game *g) {
    Node *head = NULL;
    Node *prev = NULL;
    int count = 0;

    int i = (g->mustMoveIn == ANYWHERE ? 0 : g->mustMoveIn);
    for (; i < 9; i++) {
        // Loops for every i from 0 to 8 if mustMoveIn == ANYWHERE, loops once with i = mustMoveIn otherwise (breaks at the end)
        for (int j = 0; j < 9; j++) {
            if (g->board[i][j] == EMPTY) {
                Game *new = malloc(sizeof(Game));
                initGame(new, g->depth - 1);
                memcpy(new->board, g->board, sizeof(uint8_t) * 9 * 9);
                memcpy(new->metaBoard, g->metaBoard, sizeof(uint8_t) * 9);
                new->board[i][j] = g->next;
                new->next = (g->next == X ? O : X);

                if (new->metaBoard[i] == EMPTY) {
                    new->metaBoard[i] = generalGameIsFinished(new->board[i]);
                    new->mustMoveIn = (new->metaBoard[i] != NO_RESULT ? ANYWHERE : j);
                }

                head = malloc(sizeof(Node));
                initNode(head, prev, new, count);
                prev = head;
                head = NULL;
            }
        }
        if (g->mustMoveIn != ANYWHERE) {
            break;
        }
    }

    return prev;
}

float gameEvaluation(Game *g) {
    // TODO improve this heuristic
    int count = 0;
    for (int i = 0; i < 9; i++) {
        count += g->metaBoard[i] == X;
        count -= g->metaBoard[i] == O;
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

void initNode(Node *n, Node *next, Game *g, int count) {
    n->g = g;
    n->next = next;
}