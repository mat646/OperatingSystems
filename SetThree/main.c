//
// Created by Mateusz Sokol on 22.03.18.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    pid_t child_pid;
    printf("PID glownego programu: %d\n", (int)getpid());
    child_pid = vfork();
    if(child_pid!=0) {
        printf("Proces rodzica: Proces rodzica ma pid:%d\n", (int)getpid());
        printf("Proces rodzica: Proces dziecka ma pid:%d\n", (int)child_pid);
    } else {
        printf("Proces dziecka: Proces rodzica ma pid:%d\n",(int)getppid());
        printf("Proces dziecka: Proces dziecka ma pid:%d\n",(int)getpid());
        _exit(1);
    }

    char fileName[20];
    sscanf(argv[1], "%s", fileName);

    pid_t pid = vfork();

    if(pid != 0) {
        execl(fileName, "ls");
        int status;
        wait(&status);
        exit(1);
    }



    return 0;
}