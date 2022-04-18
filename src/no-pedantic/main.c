#include "cthread.h"
#include "counters.h"
#include <stddef.h>

int main(void) {
    Cthread cb = add_cthread(beat, NULL);
    Cthread c4 = add_cthread(fours, NULL);
    Cthread c5 = add_cthread(fives, NULL);

    while (1)
        ((Handler *) next_cthread_va(3, cb, c4, c5))();
}
