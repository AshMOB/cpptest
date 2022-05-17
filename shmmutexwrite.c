#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define BUFFER_SIZE 10
#define sem_name "mysem"
int main()
{
    struct Stu
    {
        char name[10];
        int score;
    };
    int shmid;
    sem_t *sem;
    int score = 60, i = 1;
    char buff[BUFFER_SIZE];
    key_t shmkey;
    shmkey = ftok("shmmutexread.c", 0);
    sem = sem_open(sem_name, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED)
    {
        printf("unable to creat semaphore!");
        sem_unlink(sem_name); //删除有名信号量
        exit(-1);
    }
    shmid = shmget(shmkey, 1024, 0666 | IPC_CREAT);
    /*创建IPC键值为shmkey的共享内存，其大小为1024字节，允许读写*/
    if (shmid == -1)
        printf("creat shm is fail\n");
    struct Stu *addr;
    addr = (struct Stu *)shmat(shmid, 0, 0);
    if (addr == (struct Stu *)-1)
        printf("shm shmat is fail\n");
    addr->score = 0;
    printf("写进程映射的共享内存地址=%p\n", addr);
    do
    {
        sem_wait(sem);
        memset(buff, 0, BUFFER_SIZE);
        memset((addr + i)->name, 0, BUFFER_SIZE);
        printf("写进程:输入一些姓名（不超过10个字符）到共享内存(输入'quit' 退出):\n");
        if (fgets(buff, BUFFER_SIZE, stdin) == NULL)
        {
            sem_post(sem);
            break;
        }
        strncpy((addr + i)->name, buff, strlen(buff) - 1);
        (addr + i)->score = ++score;
        addr->score++;
        i++;
        sem_post(sem);
        sleep(1);
    } while (strncmp(buff, "quit", 4) != 0);
    if (shmdt(addr) == -1) /*将共享内存与当前进程断开*/
        printf("shmdt is fail\n");
    sem_close(sem);       //关闭有名信号量
    sem_unlink(sem_name); //删除有名信号量
}
