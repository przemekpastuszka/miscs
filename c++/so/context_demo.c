/*
 * Przemysław Pastuszka, et. al.
 * Systemy operacyjne 2011 / 2012
 *
 */

#include <ucontext.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>

#include "workers.h"

#define NUMCONTEXTS 10              /* how many contexts to make */
#define STACKSIZE 8192              /* stack size */
#define INTERVAL 100                /* timer interval in nanoseconds */

ucontext_t signal_context;          /* the interrupt context */
void *signal_stack;                 /* global interrupt stack */

ucontext_t contexts[NUMCONTEXTS];   /* store our context info */
int deadcons[NUMCONTEXTS];
int curcontext = 0;                 /* whats the current context? */
ucontext_t *cur_context;            /* a pointer to the current_context */

ucontext_t main_context;

int* was_preempted; // was the current context preempted by the timer
int running_contexts = NUMCONTEXTS; // nr of currently running contexts
int idle_threads; // nr of idle threads (threads, that do nothing)
int priorities[NUMCONTEXTS]; // priorities to use
int use_priorities;

void milisecitimer(int delay) {
    struct itimerval it_new, it_old;
    it_new.it_interval.tv_sec = 0;
    it_new.it_interval.tv_usec = delay * 1000;
    it_new.it_value = it_new.it_interval;
    if (setitimer(ITIMER_REAL, &it_new, &it_old) ) perror("setitiimer");
    if (delay > 0) printf("Time wasted by thread %i: %i sec, %i usec\n", curcontext, it_old.it_value.tv_sec, it_old.it_value.tv_usec);
}

/******************/
/*** SCHEDULERS ***/
/******************/
void round_robin() {
    while(running_contexts > idle_threads) { // stop when only idle threads left
        printf("scheduling out thread %d\n", curcontext);

        if(*was_preempted == 0) { // context was not preempted - it means it's finished, so mark it as dead
            deadcons[curcontext] = 1;
            --running_contexts;
            printf("Context %d is dead\n", curcontext);
        }

        do { // find next nondead (eligible for running) context
            curcontext = (curcontext + 1) % NUMCONTEXTS;
        } while(deadcons[curcontext]);
        cur_context = &contexts[curcontext];

        printf("scheduling in thread %d\n", curcontext);

        milisecitimer(INTERVAL * (use_priorities ? priorities[curcontext] : 1)); // contexts with higher priorities works longer

        *was_preempted = 0; // zero it
        swapcontext(&signal_context,cur_context);
    }
}

void first_in_first_served() {
    while(curcontext < NUMCONTEXTS) {
        printf("scheduling in thread %d\n", curcontext);
        cur_context = &contexts[curcontext];
        swapcontext(&signal_context, cur_context);
        ++curcontext;
    }
}
/******************/

void timer_interrupt(int j, siginfo_t *si, void *old_context) {
    milisecitimer(0);

    *was_preempted = 1; // context is not dead!

    swapcontext(cur_context,&signal_context);
}

void setup_signals(void) {
    struct sigaction act;

    act.sa_sigaction = timer_interrupt;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART | SA_SIGINFO;

    if(sigaction(SIGALRM, &act, NULL) != 0) {
        perror("Signal handler");
    }
}

void mkcontext_with_arg(ucontext_t *uc,  void *function, int arg) {
    prepare_context(uc);
    makecontext(uc, function, 1, arg);
}

void mkcontext(ucontext_t *uc,  void *function) {
    prepare_context(uc);
    makecontext(uc, function, 0);
}

void prepare_context(ucontext_t *uc) {
    void * stack;
    getcontext(uc);

    stack = malloc(STACKSIZE);
    if (stack == NULL) {
        perror("malloc");
        exit(1);
    }
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = STACKSIZE;
    uc->uc_stack.ss_flags = 0;
    uc->uc_link = &signal_context;
    sigemptyset(&uc->uc_sigmask);
}

void make_scheduler_context(void *function) {
    getcontext(&signal_context);
    signal_context.uc_stack.ss_sp = signal_stack;
    signal_context.uc_stack.ss_size = STACKSIZE;
    signal_context.uc_stack.ss_flags = 0;
    signal_context.uc_link = &main_context;
    sigemptyset(&signal_context.uc_sigmask);
    makecontext(&signal_context, function, 0);
}


int main() {
    was_preempted = malloc(sizeof(int));
    *was_preempted = 1;
    srand(MAGIC); // use constant rather than time(NULL) to make program deterministic

    /************************************/
    /********** BASIC SETTINGS **********/
    /************************************/
    idle_threads = 0;
    int bongo_sorts = 5;
    use_priorities = 1; // affect only round_robin scheduler
    void* scheduler = round_robin;
    /************************************/

    fprintf(stderr,"Process Id: %d\n", (int)getpid());
    signal_stack = malloc(STACKSIZE);
    if (signal_stack == NULL) {
        perror("malloc");
        exit(1);
    }

    make_scheduler_context(scheduler);

    int i;
    for(i = 0; i < idle_threads; ++i) { // make idle contexts
        mkcontext(&contexts[i], idle_thread);
        deadcons[i] = 0;
        priorities[i] = 1;
    }
    for(; i < bongo_sorts + idle_threads; ++i) { // make bongo contexts
        mkcontext(&contexts[i], bongo_sort);
        deadcons[i] = 0;
        priorities[i] = 3; // set is as you want
    }
    for(; i < NUMCONTEXTS; ++i) { // make fibonacci contexts
        mkcontext_with_arg(&contexts[i], naive_fibonacci, FIB);
        deadcons[i] = 0;
        priorities[i] = 3;
    }

    setup_signals();

    cur_context = &contexts[0];
    swapcontext(&main_context, &signal_context);

    free(was_preempted);

    // THIS DOES NOT WORK GOOD!!! (use 'time program_name' to get real execution time)
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    printf("Time spent (user mode): %d seconds, %d microseconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("Time spent (kernel mode): %d seconds, %d microseconds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);

    return 0; /* make gcc happy */
}
