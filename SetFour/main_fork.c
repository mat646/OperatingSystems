//
// Created by Mateusz Sokol on 05.04.18.
//

#include <signal.h>
#include <printf.h>
#include <stdlib.h>
#include <stdio.h>
#include <zconf.h>

int global_state;
int notAncestor = 1;
pid_t pid = 0;

void sig_stop(int signum) {
    if (global_state == 1) {
        global_state = 0;
        printf("Oczekuje na CTRL+Z - kontynuacja albo CTRL+C - zakonczenie programu\n");
        fflush(stdout);
    } else {
        global_state = 1;
    }
}

void sig_int(int signum) {
    printf("Odebrano sygnal SIGINT\n");
    _exit(0);
}

int main() {

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        printf("Blad przy sygnale SIGINT\n");
        _exit(1);
    }

    struct sigaction sigaction1;
    sigaction1.sa_handler = sig_stop;
    sigemptyset(&sigaction1.sa_mask);
    sigaction1.sa_flags = 0;

    if (sigaction(SIGTSTP, &sigaction1, NULL) == -1) {
        printf("Blad przy sygnale SIGTSTP\n");
        _exit(1);
    }

    global_state = 1;

    while (1) {
        if (global_state == 1) {
            if (notAncestor) {
                notAncestor = 0;
                pid = vfork();
                if (pid == 0) {
                    if(execlp("./date.sh", "./date.sh", NULL) == -1) {
                        perror("Blad przy uruchomieniu skryptu");
                        _exit(1);
                    }
                    _exit(0);
                }
            }
            sleep(1);
        } else {
            if(!notAncestor){
                kill(pid, SIGKILL);
                notAncestor = 1;
            }
        }
    }
}
