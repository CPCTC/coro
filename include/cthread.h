#pragma once

#include <stdint.h>

typedef void *Cthread;

Cthread add_cthread(void *(*fn)(void *), void *arg);
void *next_cthread(uint32_t num, Cthread *threads);
void *next_cthread_va(uint32_t num, ...);
void *stop_cthread(Cthread *pct);
