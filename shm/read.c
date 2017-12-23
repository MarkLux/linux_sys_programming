#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <errno.h>

int main()
{
    key_t key = ftok("/home/shm/shmtest", 0);

    if (key == -1)
    {
        perror("ftok failed: ");
    }

    int shm_id = shmget(key, 512, IPC_CREAT| 0666);

    if (shm_id == -1)
    {
        perror("shmget error: ");
    }

    void *shm_addr = (char *)shmat(shm_id, 0, 0);

    if (shm_addr == (void *)-1)
    {
        perror("shmat error");
    }

    printf("shared memeory actived at %p\n", shm_addr);

    char *str_addr = (char *)shm_addr;

    puts(str_addr);

    shmdt(shm_addr);

    return 0;
}
