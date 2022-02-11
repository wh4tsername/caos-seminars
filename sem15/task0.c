#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static volatile sig_atomic_t sigint_counter = 0;
static volatile sig_atomic_t sigterm_flag = 0;

void handler(int signum)
{
    if (signum == SIGINT) {
        ++sigint_counter;
    } else if (signum == SIGTERM) {
        sigterm_flag = 1;
    }
}

int main()
{
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = SA_RESTART;

    sigdelset(&mask, SIGINT);
    sigdelset(&mask, SIGTERM);

    sigaction(SIGINT, &act, NULL);
    sigaction(SIGTERM, &act, NULL);

    pid_t curr_pid = getpid();

    printf("%d\n", curr_pid);
    fflush(stdout);

    while (!sigterm_flag) {
        sigsuspend(&mask);
    }

    printf("%d\n", sigint_counter);

    return 0;
}
