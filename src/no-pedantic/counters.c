#include "counters.h"
#include <stdio.h>
#include <unistd.h>

#define UBEAT (1000000 / 2)

//typedef void Handler(void);

Handler beat_handler;
Handler four_handler;
Handler five_handler;

//Handler *beat(void *);
void *beat(void *) {
    usleep(1 * UBEAT);
    return (void *) beat_handler;
}

//Handler *fours(void *);
void *fours(void *) {
    usleep(4 * UBEAT);
    return (void *) four_handler;
}

//Handler *fives(void *);
void *fives(void *) {
    usleep(5 * UBEAT);
    return (void *) five_handler;
}

void beat_handler(void) {
    printf("\n");
}

void four_handler(void) {
    printf("Four!\n");
}

void five_handler(void) {
    printf("Five!\n");
}
