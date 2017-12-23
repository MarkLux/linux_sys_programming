/**
 * 共享内存实现进程通信 写者进程
 */
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <errno.h>

int main()
{
    /* 
    * ftok: 提供一个目录和一个自定义非负整数，产生一个key值，
    * 主要用于IPC中，两个通信进程之间维持一个相同的标识符
    * 注意第一个参数的目录必须存在，且该进程有访问权限，否则失败
    */
    key_t key = ftok("/home/shm/shmtest", 0);

    if (key == -1)
    {
        perror("ftok failed: ");
    }

    /*
    * int shmget(key_t key,size_t size,int flag)
    * 用于根据key，申请（创建或者读取）一块指定大小的共享内存
    * key可以使用IPC_PRIVATE来代替，不过这样的共享内存只有父子进程间可以使用
    * flag位常见IPC_CREAT,IPC_EXCL,|后面的数字表示该内存的访问权限（可以大致类比成是操作文件）
    */

    int shm_id = shmget(key, 512, IPC_CREAT | IPC_EXCL| 0666);

    if (shm_id == -1)
    {
        perror("shmget error: ");
    }

    /*
    * void * shmat(int shmid,const void *,int)
    * shmget获得共享内存访问后，使用此函数将对应的共享内存映射到本进程的地址空间中来
    * 返回映射后对应的首地址，后两个参数一般用0
    */
    void *shm_addr = (char *)shmat(shm_id, 0, 0);

    if (shm_addr == (void *)-1)
    {
        perror("shmat error");
    }

    printf("shared memeory actived at %p\n", shm_addr);

    char *str_addr = (char *)shm_addr;

    if (strcpy(str_addr, "hello world") == -1)
    {
        perror("str copy error: ");
    }
    /*
    * shmdt(void * shmaddr)
    * 用于解除共享内存的地址映射
    */
    shmdt(shm_addr);

    return 0;
}
