#include <stdio.h>	
#include <string.h>
#include <stdint.h>	/* for uint64 definition */
#include <time.h>	/* for clock_gettime */
#include <sched.h>            /* Definition of SCHED_* constants */
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <linux/sched.h>    /* Definition of struct clone_args */
#include <unistd.h>


#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

#define _GNU_SOURCE 
#define ROW (100)
#define COL ROW


int calc(int time, int cpu)
{
	int matrixA[ROW] [COL];
	int matrixB[ROW] [COL];
	int matrixC[ROW] [COL];
	int i, j, k;
	int count=0;
	int cpuid = cpu;
	int diff=0;
	int swt=0;
	struct timespec start, end;
 
	clock_gettime(CLOCK_MONOTONIC,&start); /* mark the start time */

	while(swt<time*1000)
	{
		for(i = 0; i < ROW ; i++)
		{
			for(j = 0 ;j < COL ; j++)
			{
				for(k = 0 ; k < COL ; k++)
				{
					matrixC[i][j] += matrixA[i][k]*matrixB[k][j];
				}
			}
		}

		count++; 

		clock_gettime(CLOCK_MONOTONIC, &end);		/* mark the end time */
		diff = end.tv_nsec-start.tv_nsec;

		if ((diff)%100 == 0)
		{
			printf("PROCESS #%02d count = %d 100\n", cpuid, count);
			swt += 100;
		}	
	}   	 

	printf("DONE!! PROCESS #%02d : %d %d\n", cpuid, count, diff);
	return 0;

}


int main (void)
{
	int m,n,a;
	pid_t pid;
	scanf("%d %d",&m,&n);
	struct sched_attr 
	{
   	uint32_t size;              /* Size of this structure */
  	uint32_t sched_policy;      /* Policy (SCHED_*) */
   	uint64_t sched_flags;       /* Flags */
   	int32_t  sched_nice;        /* Nice value (SCHED_OTHER,
                                  SCHED_BATCH) */
   	uint32_t sched_priority;    /* Static priority (SCHED_FIFO,
                                  SCHED_RR) */
   	/* Remaining fields are for SCHED_DEADLINE */
   	uint64_t sched_runtime;
   	uint64_t sched_deadline;
   	uint64_t sched_period;
	};
	int sched_setattr(pid_t pid,const struct sched_attr *attr,unsigned int flags) 
	{
    		return syscall(__NR_sched_setattr, pid, attr, flags);
	}

	struct sched_attr attr;
	memset(&attr,0, sizeof(attr));
	attr.size= sizeof(attr);
	attr.sched_priority=10;
	attr.sched_policy = SCHED_RR;
	a=sched_setattr(getpid(), &attr, 0);

	for (int x=0;x<m;x++)
	{
		printf("Creating Process: #%02d\n", x);
	}

	for (int y=0;y<m;y++)
	{
		pid= fork();
		
		if(pid == 0)
		{ 
			calc(n,y);
			break;
		}
	}
	
}








