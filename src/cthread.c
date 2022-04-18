#include "cthread.h"
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <poll.h>
#include <pthread.h>
#include <semaphore.h>

//

struct cthread {
    void *(* fn)(void *);
    void *arg;
    sem_t sem;
    int pipe[2];
    pthread_t tid;
};
// typedef struct cthread *Cthread;

//

// void runner(struct cthread *cthread);
void *runner(void *);

//

Cthread add_cthread(void *(*fn)(void *), void *arg) {
    struct cthread *ct = malloc(sizeof (struct cthread));
    if (!ct) return NULL;
    ct->fn = fn;
    ct->arg = arg;
    sem_init(&(ct->sem), 0, 1);
    if (pipe(ct->pipe)) goto free_ct;
    if (pthread_create(&(ct->tid), NULL, runner, (void *) ct)) goto cleanup;
    return (Cthread) ct;

cleanup:
    close(ct->pipe[0]);
    close(ct->pipe[1]);
free_ct:
    free(ct);
    return NULL;
}

// Block until one of threads accepts.
void *next_cthread(uint32_t num, Cthread *cts) {
    struct cthread **threads = (struct cthread **) cts;
    struct pollfd *fds = malloc(num * sizeof (struct pollfd));
    if (!fds) goto error;
    for (uint32_t i = 0; i < num; i++) {
        fds[i].fd = threads[i]->pipe[0];
        fds[i].events = POLLIN;
    }
    if (poll(fds, num, -1) == -1) goto error;
    for (uint32_t i = 0; i < num; i++)
        if (fds[i].revents & POLLIN) {
                void *ret;
                read(fds[i].fd, &ret, sizeof (ret));
                sem_post(&(threads[i]->sem));

                free(fds);
                return ret;
        }
error:
    free(fds);
    return NULL;
}

void *next_cthread_va(uint32_t num, ...) {
    va_list ap;
    va_start(ap, num);
    Cthread *threads = malloc(num * sizeof (Cthread));
    for (uint32_t i = 0; i < num; i++) {
        threads[i] = va_arg(ap, Cthread);
    }
    va_end(ap);
    void *ret = next_cthread(num, threads);
    free(threads);
    return ret;
}

void *stop_cthread(Cthread *arg) {
    struct cthread **pthread = (struct cthread **) arg;
    void *ret;
    read((*pthread)->pipe[0], &ret, sizeof (ret));
    pthread_cancel((*pthread)->tid);
    sem_post(&((*pthread)->sem));
    pthread_join((*pthread)->tid, NULL);
    return ret;
}

void *runner(void *args) {
    struct cthread *ct = args;

    while (1) {
        sem_wait(&(ct->sem));
        pthread_testcancel();
        void *ret = (ct->fn)(ct->arg);
        write(ct->pipe[1], &ret, sizeof (ret));
    }
}
