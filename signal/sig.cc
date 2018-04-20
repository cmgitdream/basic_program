
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/* Simple error handling functions */

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static void *
worker_thread(void *arg)
{
    for (;;) {
        printf("%s: sleep 3\n", __func__);
	sleep(3);
    }
}

static void *
sig_thread(void *arg)
{
    //pthread_detach(pthread_self());
    sigset_t *set = (sigset_t *)arg;
    int s, sig;
    int s2;
    pthread_t thread2;

    s2 = pthread_create(&thread2, NULL, &worker_thread, NULL);
    if (s2 != 0)
        handle_error_en(s2, "pthread_create for worker_thread");
    for (;;) {
        s = sigwait(set, &sig);
        if (s != 0)
            handle_error_en(s, "sigwait");
        printf("Signal handling thread got signal %d\n", sig);
    }
    pthread_join(thread2, NULL);
}


int
main(int argc, char *argv[])
{
    pthread_t thread, thread2;
    sigset_t set;
    int s, s2;

    /* Block SIGQUIT and SIGUSR1; other threads created by main()
 *        will inherit a copy of the signal mask. */
    sigemptyset(&set);
//    sigaddset(&set, SIGQUIT);
//    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGTERM);
    sigaddset(&set, SIGHUP);
    sigaddset(&set, SIGPIPE);
    s = pthread_sigmask(SIG_BLOCK, &set, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_sigmask");


    s = pthread_create(&thread, NULL, &sig_thread, (void *) &set);
    if (s != 0)
        handle_error_en(s, "pthread_create");


    /* Main thread carries on to create other threads and/or do
 *        other work */
    pthread_join(thread, NULL);
    //pthread_join(thread2, NULL);
    //pthread_kill(thread2, SIGUSR1);

    //pause();            /* Dummy pause so we can test program */
}
