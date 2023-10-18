#include <stdio.h>
#include <pthread.h>

#include "node.h"

long long unsigned int counts[128] = {0};

typedef struct ThreadArguments {
    float returnValue;
    int threadIndex;
    Node *toEvaluate;
} ThreadArguments;

float minimax(Game *g, bool isX, int i) {

    counts[i]++;

    if (g->depth < 0) {
        return gameEvaluation(g);
    }

    Game *move;
    float best = (isX ? -100.0f : 100.0f);

    while (1) {
        move = getNextMove(g);
        if (move == NULL) {
            break;
        }

        float eval = minimax(move, !isX, i);

        if (isX) {
            best = (eval > best ? eval : best);
        } else {
            best = (eval < best ? eval : best);
        }

        free(move);
    }

    return best;
}

int countMoves(Node *n) {
    int count;
    for (count = 0; n != NULL; n = n->next) {
        count++;
    }
    return count;
}

void *threadMinimax(void *args) {
    ThreadArguments *arguments = (ThreadArguments *)args;
    arguments->returnValue = minimax(arguments->toEvaluate->g, false, arguments->threadIndex); // Because X starts, so second move is O
    return NULL;
}

float startMinimax(Game *g) {
    Node *moves = getAllMoves(g);

    int count = countMoves(moves);
    if (count > 127) {
        printf("Something horrible happened.\n");
        return 0xDEADBEEF;
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * count);
    ThreadArguments *args = malloc(sizeof(ThreadArguments) * count);

    for (int i = 0; i < count; i++) {
        args[i].returnValue = -1000.0f;
        args[i].toEvaluate = moves;
        args[i].threadIndex = i;
        pthread_create(&threads[i], NULL, threadMinimax, (void *)(&args[i]));
        moves = moves->next;
    }

    for (int i = 0; i < count; i++) {
        pthread_join(threads[i], NULL);
    }

    float best = -1000.0f;
    for (int i = 0; i < count; i++) {
        best = (args[i].returnValue > best ? args[i].returnValue : best);
        free(args[i].toEvaluate->g);
        free(args[i].toEvaluate);
    }

    free(threads);
    free(args);

    return best;
}

int main() {
    printf("super-ttt\n");

    Game initial;
    initGame(&initial, 5);

    float e = startMinimax(&initial);
    printf("%f\n", e);
    long long unsigned int total = 0;
    for (int i = 0; i < 128; i++) {
        total += counts[i];
    }
    printf("Analyzed %llu positions\n", total);

    return 0;
}