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
#define sem_name "mysem"
int main()
{
    int shmid;
    sem_t *sem;
    int i = 1;
    key_t shmkey;
    shmkey = ftok("shmmutexread.c", 0);
    struct Stu
    {
        char name[10];
        int score;
    };
    sem = sem_open(sem_name, 0, 0644, 0);
    if (sem == SEM_FAILED)

    {
        printf("unable to open semaphore!");
        sem_close(sem);
        exit(-1);
    }
    shmid = shmget(shmkey, 0, 0666);
    if (shmid == -1)
    {
        printf("creat shm is fail\n");
        exit(0);
    }
    struct Stu *addr;
    addr = (struct Stu *)shmat(shmid, 0, 0);
    if (addr == (struct Stu *)-1)
    {
        printf("shm shmat is fail\n");
        exit(0);
    }
    printf("读进程映射的共享内存地址=%p\n", addr);
    do
    {
        sem_wait(sem);

        if (addr->score > 0)

        {
            printf("\n读进程:绑定到共享内存 %p:姓名 %d   %s , 分值%d \n", addr, i, (addr + i)->name, (addr + i)->score);
            addr->score--;
            if (strncmp((addr + i)->name, "quit", 4) == 0)
                break;
            i++;
        }
        sem_post(sem);
    } while (1);
    sem_close(sem);
    if (shmdt(addr) == -1)
        printf("shmdt is fail\n");
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        printf("shmctl delete error\n");
}
