#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct _msg_buf
{                  //消息结构体
    long type;     //消息类型
    char buf[100]; //消息内容
} msg_buf;
int main()
{
    int key, qid;
    int wc = 1;
    msg_buf buf;
    key = ftok(".", 10);
    qid = msgget(key, IPC_CREAT | 0666);
    printf("key: %d\nqid: %d\n", key, qid);
    buf.type = 10;
    system("ipcs -q");
    printf("请输入一些消息，每条消息以回车结束。如果输入quit，则程序结束\n");
    while (1)
    { //循环输入消息到队列
        printf("输入第%d条消息：", wc++);
        fgets(buf.buf, 100, stdin); //从键盘输入不超过100个字符的消息到消息缓冲区
        if (strncmp(buf.buf, "quit", 4) == 0)
        {                                          //如果输入"quit"，则删除消息队列并结束程序
            if ((msgctl(qid, IPC_RMID, NULL)) < 0) /*删除指定的消息队列*/
            {
                exit(1);
            }
            else
            {
                system("ipcs -q");
                printf("successfully removed %d queue/n", qid); /* 删除队列成功 */
                exit(0);
            }
        }
        if (msgsnd(qid, (void *)&buf, 100, 0) < 0)
        { //发送消息缓冲区中的信息到消息队列
            perror("msgsnd");
            exit(-1);
        }
    }
    return 0;
}