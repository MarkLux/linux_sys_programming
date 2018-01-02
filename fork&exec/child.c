#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    

    int shmid = -1;

    if (argc > 1)
    {
        char *shmid_str = argv[1];
        shmid = atoi(shmid_str);

        printf("get shmid: %d\n", shmid);
    }

    printf("child.c is calling\n");

    pid_t child_pid = fork();
    struct rusage child_usage;
    int child_status;

    if (child_pid > 0)
    {
        wait4(child_pid, &child_status, WSTOPPED, &child_usage);
        char *sharepath = "/home/shm/testshm2";
        key_t key = ftok(sharepath, 0);
        if (key == -1)
        {
            perror("[child] ftok failed: ");
        }
        int shmid = shmget(key, 512, IPC_CREAT | IPC_EXCL | 0666);
        if (shmid == -1)
        {
            perror("[child] shmget error: ");
        }
        void *shmaddr = shmat(shmid, 0, 0);
        if (shmaddr == (void *)-1)
        {
            perror("[child] shmat error: ");
        }
        printf("[child] shmaddr: %p\n",shmaddr);
        char * straddr = (char *)shmaddr;
        sprintf(straddr, "%ld", child_usage.ru_maxrss);
        shmdt(shmaddr); // to forbid process to access the shm
        printf("child process exited with %ld k memory used\n", child_usage.ru_maxrss);
    }
    else if (child_pid == 0)
    {
        // child process
        int size = 1 * 1024 * 1024;
        int *a = NULL;
        a = (int *)malloc(size);
        memset(a, 1, size);
    }

    return 0;
}