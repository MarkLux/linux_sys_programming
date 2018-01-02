#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    // get();
    int size = 80 * 1024 * 1024;
    int *a = NULL;
    a = (int *)malloc(size);
    memset(a, 1, size);

    int child_id = fork();

    if (child_id == 0)
    {
        printf("child process calling\n");

        // char shmid_str[16];
        // // int shmid = 100;
        // sprintf(shmid_str, "%d", shmid);
        // shmid_str = "100";

        // char *args[] = {"/home/judge/test/test_child", shmid_str, NULL};
        execve("/home/judge/shm/child", NULL, NULL);
    }
    else if (child_id > 0)
    {
        int status;
        struct rusage usg;
        wait4(child_id, &status, WSTOPPED, &usg);
        printf("child id: %d\n", child_id);
        printf("father process calling\n");
        printf("main father waited the child ended, with %ld k memeory used.\n", usg.ru_maxrss);

        int shmid;
        char *shmaddr;
        char *sharepath = "/home/shm/testshm2";
        key_t key = ftok(sharepath, 0);
        if (key == -1)
        {
            perror("[father] ftok error: ");
            return 0;
        }
        shmid = shmget(key, 512, IPC_CREAT | 0666);
        if (shmid == -1)
        {
            perror("[father] shmget error: ");
        }

        shmaddr = shmat(shmid, 0, 0);
        if (shmaddr == (void *)(-1))
        {
            perror("[father] shmat error: ");
        }
        puts((char *)shmaddr);
        shmdt(shmaddr);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}