#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define EMPTY 0
#define NO_RESULT 0
#define P_X 1
#define P_O 2
#define TIE 3

#define ANYWHERE -1
#define FINISHED_SEARCH -2

typedef struct Game {
    uint8_t board[9][9];
    uint8_t metaBoard[9];
    uint8_t nextPl; // Who is to move
    int8_t nextMoveMetaPtr; // From where metagame (0-9) to start search for next move (necessary for getNextMove)
    int8_t nextMovePtr; // Same, but in subgame
    int8_t mustMoveIn; // Player must move in which square
    int depth; // Current minimax search depth

} Game;

typedef struct Node {
    Game *g;
    struct Node *next;
} Node;

void initGame(Game *g, int depth);
Game *getNextMove(Game *g);
Node *getAllMoves(Game *g);
float gameEvaluation(Game *g);
bool isTerminal(Game *g);
int generalGameIsFinished(uint8_t *g);

void initNode(Node *n, Node *prev, Game *g);

#endif // ifndef GAME_H