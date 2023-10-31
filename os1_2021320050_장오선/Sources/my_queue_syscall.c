#include<linux/syscalls.h>
#include<linux/kernel.h>
#include<linux/linkage.h>

#define MAXSIZE 500

int queue[MAXSIZE];
int front = 0;
int rear = 0;
int i,j,res = 0;

SYSCALL_DEFINE1(oslab_enqueue, int, a)// argument:system call name, input datatype, input variable
{
	if(rear >= MAXSIZE - 1) //if queue full
	{ 
		printk(KERN_INFO "[Error] - QUEUE IS FULL------------\n");
		return -2;
	}
	for ( i = 0; i<=rear ;i++) // queue is not full, we check the value if they are equal one by one with for loop
	{
		if (a == queue[i])// if new input is redundant
		{	
			printk(KERN_INFO "[Error] - Already existing value \n");
			return a;
		}
	}
	
	queue[rear] = a ;// if new input makes sense
	
	printk(KERN_INFO "[System call] oslab_enqueue(); ------\n");// use I/O to visualize our queue
	printk("Queue Front--------------------\n");
	for (i = front; i <= rear ; i ++)// print them one by one
		printk("%d\n", queue[i]);
	printk("Queue Rear--------------------\n");
	
	rear = rear + 1;// increase the queue size by 1

	return a;// return the input new element in queue
}


SYSCALL_DEFINE0(oslab_dequeue)// 0 means no input here just as what we define in the head file before
{
	if(rear == front) // if queue is empty
	{ 
		printk(KERN_INFO "[Error]- EMPTY QUEUE------------\n");
		return -2;
	}
	res = queue[front];// choose the element which is going to be dequeued
	rear = rear - 1;// decrease queue size to implement the dequeue operation
	
	for(i = front + 1; i < MAXSIZE ; i++)// since we dequeue the 1st element in queue, we have to move all the others one place on
		queue[i-1] = queue[i];

	printk(KERN_INFO "[System call] oslab_dequeue(); ------\n");// use I/O to visualize our queue
	printk("Queue Front--------------------\n");	

	for (j = front; j < rear ; j ++)// print them one by one
		printk("%d\n", queue[j]);
	
	printk("Queue Rear--------------------\n");

	return res;// return the dequeued element
}
