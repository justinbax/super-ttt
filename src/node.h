#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define EMPTY 0
#define NO_RESULT 0
#define X 1
#define O 2
#define TIE 3

#define ANYWHERE -1

typedef struct Game {
    uint8_t board[9][9];
    uint8_t metaBoard[9];
    uint8_t next;
    int depth;
    int8_t mustMoveIn;
} Game;

typedef struct Node {
    Game *g;
    struct Node *next;
} Node;

void initGame(Game *g, int depth);
Node *possibleMoves(Game *g);
float gameEvaluation(Game *g);
bool isTerminal(Game *g);
int generalGameIsFinished(uint8_t *g);

void initNode(Node *n, Node *prev, Game *g);

#endif // ifndef GAME_H