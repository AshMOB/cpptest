#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#define Maxbuf 10		//缓冲单元数目
#define TimesOfOp 10		//生产者、消费者循环读写缓冲区的次数
#define true 1
#define false 0
#define historynum 100	//生产者、消费者读写历史记录数目
struct Circlebuf	//循环缓冲队列结构
{
	int read; 			//读指针
	int write; 		//写指针
	int buf[Maxbuf]; 	//缓冲区
} circlebuf;
sem_t mutex;			//互斥信号量
sem_t empty; 			//空白缓冲区同步信号量
sem_t full; 			//满缓冲区同步信号量
char writehistory[historynum][30];//写历史
char readhistory[historynum][30];//读历史
int writehistorycount=0;		//写历史计数器
int readhistorycount=0; 		//读历史计数器
char history[historynum][30];	//缓冲区操作历史
int historycount=0;		//缓冲区操作历史计数器
void writeCirclebuf(struct Circlebuf *circlebuf,int *value) //向缓冲区中写一个值
{
	circlebuf->buf[circlebuf->write]=(*value);
	sleep(1);							
	circlebuf->write=(circlebuf->write+1)%Maxbuf; 
}
int readCirclebuf(struct Circlebuf *circlebuf)
{
	int value=0;
	value=circlebuf->buf[circlebuf->read];		
sleep(1);								
circlebuf->buf[circlebuf->read]=0; 			
circlebuf->read=(circlebuf->read+1)%Maxbuf; 	
return value;
}
void sigend(int sig)
{
	exit(0);
}
void * productThread(void *i)
{
	int *n=(int *)i;
	int t=TimesOfOp;
	int writeptr;
    while(t--)
	{	sem_wait(&empty);
		sem_wait(&mutex);
		writeCirclebuf(&circlebuf,n);
		if(circlebuf.write>0) writeptr=circlebuf.write-1;
		else writeptr= Maxbuf-1;
	sprintf(writehistory[writehistorycount++],"生产者%d:缓冲区%d=%d", *n,writeptr,*n);
		sprintf(history[historycount++],"生产者%d:缓冲区%d=%d\n", *n,writeptr, *n);
		sem_post(&mutex);
		sem_post(&full);
		sleep(1);
	}
}
void * consumerThread(void *i)
{
	int *n=(int *)i;
	int t=TimesOfOp;						
int value=0;							
int readptr;							
while(t--)
	{
		sem_wait(&full);						
sem_wait(&mutex);
		value=readCirclebuf(&circlebuf);			
if(circlebuf.read>0) readptr=circlebuf.read-1;	
		else readptr= Maxbuf-1;
        sprintf(readhistory[readhistorycount++], "消费者%d:缓冲区%d=%d\n", *n,readptr,value);
		sprintf(history[historycount++],"消费者%d:缓冲区%d=%d\n", *n,readptr,value);
		sem_post(&mutex);						
sem_post(&empty);						
sleep(1);							
}
}
int main()
{
	int i,max;
	int ConsNum=0,ProdNum=0,ret;
	sem_init(&mutex,0,1);
    sem_init(&empty,0,Maxbuf);
	sem_init(&full,0,0);	
	signal(SIGINT, sigend);
	signal(SIGTERM, sigend);
	circlebuf.read=circlebuf.write=0;
	for(i=0;i<Maxbuf;i++)
		circlebuf.buf[i]=0;
	printf("请输入生产者线程的数目 :");
	scanf("%d",&ProdNum);
	int *pro=(int*)malloc(ProdNum*sizeof(int));
	pthread_t *proid=(pthread_t*)malloc(ProdNum*sizeof(pthread_t));
  	printf("请输入消费者线程的数目 :");
 	scanf("%d",&ConsNum);
	int *con=(int*)malloc(ConsNum*sizeof(int));
    pthread_t *conid=(pthread_t*)malloc(ConsNum*sizeof(pthread_t));
	for(i=1;i<=ConsNum;i++)
	{
		con[i-1]=i;
	ret=pthread_create(&conid[i],NULL,consumerThread,(void *)&con[i-1]);
		if(ret!=0)
		{
			printf("Create thread error");
			exit(1);
		}
	}
for(i=1;i<=ProdNum;i++)
	{
		pro[i-1]=i;
	ret=pthread_create(&proid[i],NULL,productThread,(void *)&pro[i-1]);
		if(ret!=0)
		{
			printf("Create thread error");
			exit(1);
		}
	}
	sleep((ConsNum+ ProdNum)*10);
	if (writehistorycount>readhistorycount) max=writehistorycount;
	else max=readhistorycount;
for(i=0;i<max;i++)
		if ((i<writehistorycount) && (i<readhistorycount))
			printf("%s | %s\n",writehistory[i],readhistory[i]);
		else if (i<writehistorycount)
			printf("%s | %s\n",writehistory[i]," ");
		else 	printf("%s | %s\n"," ",readhistory[i]);
	printf("*************缓冲池的操作历史为：******************\n");
	for(i=0;i<historycount;i++)  printf("%s",history[i]);
	sem_destroy(&mutex);	
	sem_destroy(&empty);
	sem_destroy(&full);
}
